#pragma once

#include <nlohmann/json.hpp>
#include <string>

namespace smart_json::adapter {

template<>
struct Adapter<nlohmann::json, void>
{
	template<typename Type>
	static Type get(const nlohmann::json& json)
	{
		return json.get<Type>();
	}
	static const nlohmann::json& as_array(const nlohmann::json& json)
	{
		return json;
	}
	static const nlohmann::json& as_object(const nlohmann::json& json)
	{
		return json;
	}
	static bool is_null(const nlohmann::json& json)
	{
		return json.is_null();
	}
	static const nlohmann::json* find(const nlohmann::json& json, const char* name)
	{
		if (const auto it = json.find(name); it != json.end()) {
			return &*it;
		}
		return nullptr;
	}
	static const nlohmann::json& to_element(const nlohmann::json::const_iterator& json)
	{
		return *json;
	}
	static std::pair<std::string, const nlohmann::json&>
	  to_key_value(const nlohmann::json::const_iterator& pair)
	{
		return { pair.key(), pair.value() };
	}
	template<typename Type>
	static void put(nlohmann::json& json, const Type& value)
	{
		json = value;
	}
	static nlohmann::json make_element()
	{
		return {};
	}
	static void push(nlohmann::json& json, nlohmann::json&& element)
	{
		json.push_back(std::move(element));
	}
	static nlohmann::json make_mapped()
	{
		return {};
	}
	template<typename Key>
	static void insert(nlohmann::json& json, const Key& key, nlohmann::json&& mapped)
	{
		json[key] = std::move(mapped);
	}
};

} // namespace smart_json::adapter
