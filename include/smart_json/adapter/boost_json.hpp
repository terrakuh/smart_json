#pragma once

#include <boost/json.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <string>
#include <type_traits>

namespace smart_json::adapter {

template<typename JSON>
struct Adapter<JSON, typename std::enable_if<std::is_same<JSON, boost::json::value>::value ||
                                             std::is_same<JSON, boost::json::object>::value ||
                                             std::is_same<JSON, boost::json::array>::value ||
                                             std::is_same<JSON, boost::json::string>::value>::type>
{
	template<typename Type>
	static Type get(const boost::json::value& json)
	{
		static_assert(std::is_arithmetic<Type>::value || std::is_same<Type, std::string>::value,
		              "unsupported Type");

		if constexpr (std::is_same<Type, bool>::value) {
			return json.as_bool();
		} else if constexpr (std::is_signed<Type>::value) {
			if (const auto ptr = json.if_double()) {
				return boost::numeric_cast<Type>(*ptr);
			} else if (const auto ptr = json.if_uint64()) {
				return boost::numeric_cast<Type>(*ptr);
			}
			return boost::numeric_cast<Type>(json.as_int64());
		} else if constexpr (std::is_unsigned<Type>::value) {
			if (const auto ptr = json.if_double()) {
				return boost::numeric_cast<Type>(*ptr);
			} else if (const auto ptr = json.if_int64()) {
				return boost::numeric_cast<Type>(*ptr);
			}
			return boost::numeric_cast<Type>(json.as_uint64());
		} else if constexpr (std::is_floating_point<Type>::value) {
			if (const auto ptr = json.if_int64()) {
				return boost::numeric_cast<Type>(*ptr);
			} else if (const auto ptr = json.if_uint64()) {
				return boost::numeric_cast<Type>(*ptr);
			}
			return boost::numeric_cast<Type>(json.as_double());
		} else {
			const auto& str = json.as_string();
			return std::string{ str.c_str(), str.size() };
		}
	}
	static const boost::json::array& as_array(const boost::json::value& json)
	{
		return json.as_array();
	}
	static const boost::json::array& as_array(const boost::json::array& json)
	{
		return json;
	}
	static const boost::json::object& as_object(const boost::json::value& json)
	{
		return json.as_object();
	}
	static const boost::json::object& as_object(const boost::json::object& json)
	{
		return json;
	}
	static bool is_null(const boost::json::value& json)
	{
		return json.is_null();
	}
	static const boost::json::value* find(const boost::json::object& json, const char* name)
	{
		if (const auto it = json.find(name); it != json.end()) {
			return &it->value();
		}
		return nullptr;
	}
	static const boost::json::value& to_element(const boost::json::array::const_iterator& json)
	{
		return *json;
	}
	static std::pair<std::string, const boost::json::value&>
	  to_key_value(const boost::json::object::const_iterator& pair)
	{
		return { pair->key_c_str(), pair->value() };
	}
	template<typename Type>
	static void put(boost::json::value& json, const Type& value)
	{
		json = boost::json::value_from(value);
	}
	static boost::json::value make_element()
	{
		return {};
	}
	static void emplace_array(boost::json::value& json)
	{
		json.emplace_array();
	}
	static void push(boost::json::array& json, boost::json::value&& element)
	{
		json.push_back(std::move(element));
	}
	static void push(boost::json::value& json, boost::json::value&& element)
	{
		push(json.is_null() ? json.emplace_array() : json.as_array(), std::move(element));
	}
	static boost::json::value make_mapped()
	{
		return {};
	}
	static void emplace_object(boost::json::value& json)
	{
		json.emplace_object();
	}
	template<typename Key>
	static void insert(boost::json::object& json, const Key& key, boost::json::value&& mapped)
	{
		json[key] = std::move(mapped);
	}
	template<typename Key>
	static void insert(boost::json::value& json, const Key& key, boost::json::value&& mapped)
	{
		insert(json.is_null() ? json.emplace_object() : json.as_object(), key, std::move(mapped));
	}
};

} // namespace smart_json::adapter
