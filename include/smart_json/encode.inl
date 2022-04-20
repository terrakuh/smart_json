#pragma once

#include "encode.hpp"

#include <boost/mp11.hpp>

namespace smart_json {

template<typename Primitive, typename Transformer>
inline typename std::enable_if<detail::Is_primitive<Primitive>::value, boost::json::value>::type
  encode(const Primitive& value, const Transformer& transformer)
{
	return boost::json::value_from(value);
}

template<typename Enum, typename Transformer>
inline
  typename std::enable_if<boost::describe::has_describe_enumerators<Enum>::value, boost::json::string>::type
  encode(const Enum& value, const Transformer& transformer)
{
	return boost::describe::enum_to_string(value, nullptr);
}

template<typename Type, typename Transformer>
inline
  typename std::enable_if<detail::Is_container<Type>::value && !detail::Is_associative_container<Type>::value,
                          boost::json::array>::type
  encode(const Type& value, const Transformer& transformer)
{
	boost::json::array array;
	for (const auto& el : value) {
		array.push_back(encode(el, transformer));
	}
	return array;
}

template<typename Type, typename Transformer>
inline typename std::enable_if<detail::Is_associative_container<Type>::value, boost::json::object>::type
  encode(const Type& value, const Transformer& transformer)
{
	boost::json::object object;
	for (const auto& [key, el] : value) {
		object[key] = encode(el, transformer);
	}
	return object;
}

template<typename Type, typename Transformer>
inline boost::json::value encode(const std::optional<Type>& value, const Transformer& transformer)
{
	if (value.has_value()) {
		return encode(value.value(), transformer);
	}
	return {};
}

template<typename Object, typename Transformer>
inline
  typename std::enable_if<boost::describe::has_describe_members<Object>::value, boost::json::object>::type
  encode(const Object& value, const Transformer& transformer)
{
	boost::json::object object;
	using Members = boost::describe::describe_members<Object, boost::describe::mod_any_access>;
	boost::mp11::mp_for_each<Members>([&](auto member) {
		constexpr auto name  = Transformer::template normalize<Transformer::length(member.name)>(member.name);
		object[name.c_str()] = encode(value.*member.pointer, transformer);
	});
	return object;
}

} // namespace smart_json
