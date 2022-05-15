#pragma once

#include <array>
#include <list>
#include <map>
#include <optional>
#include <queue>
#include <set>
#include <smart_json/smart_json.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

BOOST_DEFINE_ENUM_CLASS(Log_level, off, debug, info, error);

struct Config {
	std::string log_path;
	Log_level log_level;
	std::map<std::string, std::string> additional_headers;
	std::optional<std::string> save_path;
	std::optional<std::string> real_save_path;

	struct Nested {
		int value = 0;
	};

	std::array<Nested, 3> fixed_array;
	std::vector<Nested> array;
};
BOOST_DESCRIBE_STRUCT(Config, (),
                      (log_path, log_level, additional_headers, save_path, real_save_path, fixed_array,
                       array));
BOOST_DESCRIBE_STRUCT(Config::Nested, (), (value));
