#pragma once

#include <boost/property_tree/ptree.hpp>
#include <string>

namespace smart_json::adapter {

template<>
struct Adapter<boost::property_tree::ptree, void>
{
	template<typename Type>
	static Type get(const boost::property_tree::ptree& json)
	{
		return json.get_value<Type>();
	}
	static const boost::property_tree::ptree& as_array(const boost::property_tree::ptree& json)
	{
		return json;
	}
	static const boost::property_tree::ptree& as_object(const boost::property_tree::ptree& json)
	{
		return json;
	}
	static bool is_null(const boost::property_tree::ptree& json)
	{
		return false;
	}
	static const boost::property_tree::ptree* find(const boost::property_tree::ptree& json, const char* name)
	{
		if (const auto it = json.find(name); it != json.not_found()) {
			return &it->second;
		}
		return nullptr;
	}
	static const boost::property_tree::ptree&
	  to_element(const boost::property_tree::ptree::const_iterator& json)
	{
		return json->second;
	}
	static std::pair<std::string, const boost::property_tree::ptree&>
	  to_key_value(const boost::property_tree::ptree::const_iterator& pair)
	{
		return { pair->first, pair->second };
	}
	template<typename Type>
	static void put(boost::property_tree::ptree& json, const Type& value)
	{
		json.put_value(value);
	}
	static boost::property_tree::ptree make_element()
	{
		return {};
	}
	static void push(boost::property_tree::ptree& json, boost::property_tree::ptree&& element)
	{
		json.push_back(std::make_pair("", std::move(element)));
	}
	static boost::property_tree::ptree make_mapped()
	{
		return {};
	}
	template<typename Key>
	static void insert(boost::property_tree::ptree& json, const Key& key, boost::property_tree::ptree&& mapped)
	{
		json.push_back(std::make_pair(key, std::move(mapped)));
	}
};

} // namespace smart_json::adapter
