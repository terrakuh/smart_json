#include <boost/json/src.hpp>
#include <iostream>
#include <map>
#include <smart_json/smart_json.hpp>
#include <vector>

struct Upper_case_transformer
{
	/// Calculates the final string length of the normalized name.
	constexpr static std::size_t length(const char* str) noexcept
	{
		return std::char_traits<char>::length(str);
	}

	/// Normalizes the input `name` and returns a zero-terminated string as an array. `Length` must be equal to
	/// length(`name`).
	template<std::size_t Length>
	constexpr static smart_json::String_container<Length> normalize(const char* name)
	{
		smart_json::String_container<Length> str;
		bool make_uppercase = false;
		for (std::size_t i = 0; name[i] != 0; ++i) {
			if (i >= Length) {
				throw std::system_error{ smart_json::Code::bad_input_length };
			} else if (const char c = name[i]; c >= 'a' && c <= 'z') {
				str[i] = c - 'a' + 'A';
			} else {
				str[i] = c;
			}
		}
		return str;
	}
};

BOOST_DEFINE_ENUM_CLASS(Status, delivered, pending, cancelled);
struct Config
{
	std::string version;
	int something;
	std::map<std::string, std::string> map;
	struct Log
	{
		int level;
		std::string path;
	} log;
	std::vector<Log> logs;
	std::array<Log, 3> some_logs;
	std::optional<Log> something_optional;

	Status status = Status::pending;
};
BOOST_DESCRIBE_STRUCT(Config, (),
                      (version, something, log, logs, some_logs, something_optional, map, status));
BOOST_DESCRIBE_STRUCT(Config::Log, (), (level, path));

int main()
{
	auto obj = smart_json::encode<boost::json::object>(Config{ "v0.0.0", 123, { { "hi", "yo" } } },
	                                                   Upper_case_transformer{});

	std::cout << boost::json::serialize(obj) << "\n";
	auto config = smart_json::decode<Config>(obj, Upper_case_transformer{});
}
