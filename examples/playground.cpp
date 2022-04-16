#include <boost/json/src.hpp>
#include <iostream>
#include <map>
#include <smart_json/smart_json.hpp>
#include <vector>

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
	boost::json::value obj;
	smart_json::encode(Config{ "v0.0.0", 123, { { "hi", "yo" } } }, obj);

	std::cout << boost::json::serialize(obj) << "\n";
	Config config;
	smart_json::decode(config, obj);
}
