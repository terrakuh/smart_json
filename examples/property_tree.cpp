#include <array>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <map>
#include <optional>
#include <smart_json/smart_json.hpp>
#include <string>
#include <vector>

BOOST_DEFINE_ENUM_CLASS(Log_level, off, debug, info, error);

struct Config {
	std::string log_path;
	Log_level log_level;
	std::map<std::string, std::string> additional_headers;
	std::optional<std::string> save_path;

	struct Nested {
		int value = 0;
	};
	std::array<Nested, 3> fixed_array;
	std::vector<Nested> array;
};
BOOST_DESCRIBE_STRUCT(Config, (), (log_path, log_level, additional_headers, save_path, fixed_array, array));
BOOST_DESCRIBE_STRUCT(Config::Nested, (), (value));

int main()
{
	Config config{
		"v0.0.0",
		Log_level::info,
		{ { "User-Agent", "smart_json" } },
		std::nullopt,
	};
	config.fixed_array[0].value = 1;
	config.fixed_array[1].value = 2;
	config.fixed_array[2].value = 3;
	config.array.push_back({ 61 });
	auto obj = smart_json::encode<boost::property_tree::ptree>(config);
	boost::property_tree::write_json(std::cout, obj);
	std::cout << "\n";

	config = smart_json::decode<Config>(obj);
}
