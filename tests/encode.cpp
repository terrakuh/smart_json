#include "config.hpp"

#include <boost/json/src.hpp>
#include <catch2/catch.hpp>

inline Config make_config()
{
	Config config{
		"v1.2.3",     Log_level::info, { { "User-Agent", "smart_json" }, { "Location", "Potato" } },
		std::nullopt, "this is real",
	};
	config.fixed_array[0].value = 11;
	config.fixed_array[1].value = 22;
	config.fixed_array[2].value = 33;
	config.array.push_back({ 61 });
	config.array.push_back({ 123 });
	return config;
}

TEST_CASE("encode() for Boost.JSON")
{
	SECTION("custom object")
	{
		const auto json = smart_json::encode<boost::json::object>(make_config());
		REQUIRE(json.contains("logPath"));
		REQUIRE(json.at("logPath") == "v1.2.3");
		REQUIRE(json.contains("logLevel"));
		REQUIRE(json.at("logLevel") == "info");
		REQUIRE(json.contains("additionalHeaders"));
		REQUIRE(json.at("additionalHeaders").is_object());
		const auto& obj = json.at("additionalHeaders").as_object();
		REQUIRE(obj.size() == 2);
		REQUIRE(obj.contains("User-Agent"));
		REQUIRE(obj.at("User-Agent") == "smart_json");
		REQUIRE(obj.contains("Location"));
		REQUIRE(obj.at("Location") == "Potato");
		REQUIRE(!json.contains("savePath"));
		REQUIRE(json.contains("realSavePath"));
		REQUIRE(json.at("realSavePath") == "this is real");
		REQUIRE(json.contains("fixedArray"));
		REQUIRE(json.at("fixedArray").is_array());
		const auto& fixed = json.at("fixedArray").as_array();
		REQUIRE(fixed.size() == 3);
		const auto check_value = [](const boost::json::value& obj, int val) {
			REQUIRE(obj.is_object());
			REQUIRE(obj.as_object().contains("value"));
			REQUIRE(obj.as_object().at("value") == val);
		};
		check_value(fixed.at(0), 11);
		check_value(fixed.at(1), 22);
		check_value(fixed.at(2), 33);
		REQUIRE(json.contains("array"));
		REQUIRE(json.at("array").is_array());
		const auto& array = json.at("array").as_array();
		REQUIRE(array.size() == 2);
		check_value(array.at(0), 61);
		check_value(array.at(1), 123);
	}

	SECTION("std::set<std::string>")
	{
		const auto json =
		  smart_json::encode<boost::json::array>(std::set<std::string>{ "hi", "bye", "wow", "hi" });
		REQUIRE(json.size() == 3);
		// since a set is ordered
		REQUIRE(json.at(0).is_string());
		REQUIRE(json.at(0).get_string() == "bye");
		REQUIRE(json.at(1).is_string());
		REQUIRE(json.at(1).get_string() == "hi");
		REQUIRE(json.at(2).is_string());
		REQUIRE(json.at(2).get_string() == "wow");
	}
}
