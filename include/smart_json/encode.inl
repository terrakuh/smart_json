#pragma once

#include "encode.hpp"

#include <boost/mp11.hpp>

namespace smart_json {

template<typename Primitive>
inline typename std::enable_if<detail::Is_primitive<Primitive>::value>::type encode(const Primitive& value,
                                                                                    boost::json::value& json)
{
	json = value;
}

template<typename Enum>
inline typename std::enable_if<boost::describe::has_describe_enumerators<Enum>::value>::type
  encode(const Enum& value, boost::json::value& json)
{
	json = boost::describe::enum_to_string(value, nullptr);
}

template<typename Type>
inline typename std::enable_if<detail::Is_container<Type>::value &&
                               !detail::Is_associative_container<Type>::value>::type
  encode(const Type& value, boost::json::value& json)
{
	auto& array = json.emplace_array();
	for (const auto& el : value) {
		boost::json::value val;
		encode(el, val);
		array.push_back(std::move(val));
	}
}

template<typename Type>
inline typename std::enable_if<detail::Is_associative_container<Type>::value>::type
  encode(const Type& value, boost::json::value& json)
{
	auto& object = json.emplace_object();
	for (const auto& [key, el] : value) {
		boost::json::value val;
		encode(el, val);
		object[key] = std::move(val);
	}
}

template<typename Type>
inline void encode(const std::optional<Type>& value, boost::json::value& json)
{
	if (value.has_value()) {
		encode(value.value(), json);
	}
}

template<typename Object>
inline typename std::enable_if<boost::describe::has_describe_members<Object>::value>::type
  encode(const Object& value, boost::json::value& json)
{
	auto& object  = json.emplace_object();
	using Members = boost::describe::describe_members<Object, boost::describe::mod_any_access>;
	boost::mp11::mp_for_each<Members>([&](auto member) { encode(value.*member.pointer, object[member.name]); });
}

} // namespace smart_json
