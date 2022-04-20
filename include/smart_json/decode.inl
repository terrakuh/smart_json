#pragma once

#include "decode.hpp"
#include "detail/has_access.hpp"
#include "error.hpp"

#include <boost/mp11.hpp>
#include <boost/numeric/conversion/cast.hpp>

namespace smart_json {

template<typename Primitive, typename Transformer>
inline typename std::enable_if<detail::Is_primitive<Primitive>::value, Primitive>::type
  decode(const boost::json::value& json, const Transformer& transformer)
{
	if constexpr (std::is_same<Primitive, bool>::value) {
		return json.as_bool();
	} else if constexpr (std::is_signed<Primitive>::value) {
		if (const auto ptr = json.if_double()) {
			return boost::numeric_cast<Primitive>(*ptr);
		} else if (const auto ptr = json.if_uint64()) {
			return boost::numeric_cast<Primitive>(*ptr);
		}
		return boost::numeric_cast<Primitive>(json.as_int64());
	} else if constexpr (std::is_unsigned<Primitive>::value) {
		if (const auto ptr = json.if_double()) {
			return boost::numeric_cast<Primitive>(*ptr);
		} else if (const auto ptr = json.if_int64()) {
			return boost::numeric_cast<Primitive>(*ptr);
		}
		return boost::numeric_cast<Primitive>(json.as_uint64());
	} else if constexpr (std::is_floating_point<Primitive>::value) {
		if (const auto ptr = json.if_int64()) {
			return boost::numeric_cast<Primitive>(*ptr);
		} else if (const auto ptr = json.if_uint64()) {
			return boost::numeric_cast<Primitive>(*ptr);
		}
		return boost::numeric_cast<Primitive>(json.as_double());
	} else {
		const auto& str = json.as_string();
		return std::string{ str.c_str(), str.size() };
	}
}

template<typename Enum, typename Transformer>
inline typename std::enable_if<boost::describe::has_describe_enumerators<Enum>::value, Enum>::type
  decode(const boost::json::value& json, const Transformer& transformer)
{
	return decode<Enum>(json.as_string(), transformer);
}

template<typename Enum, typename Transformer>
inline typename std::enable_if<boost::describe::has_describe_enumerators<Enum>::value, Enum>::type
  decode(const boost::json::string& json, const Transformer& transformer)
{
	if (Enum value; boost::describe::enum_from_string(json.c_str(), value)) {
		return value;
	}
	throw std::system_error{ Code::unknown_enum_value, json.c_str() };
}

template<typename Type, typename Transformer>
inline
  typename std::enable_if<detail::Is_container<Type>::value && !detail::Is_associative_container<Type>::value,
                          Type>::type
  decode(const boost::json::value& json, const Transformer& transformer)
{
	return decode<Type>(json.as_array(), transformer);
}

template<typename Type, typename Transformer>
inline
  typename std::enable_if<detail::Is_container<Type>::value && !detail::Is_associative_container<Type>::value,
                          Type>::type
  decode(const boost::json::array& json, const Transformer& transformer)
{
	constexpr bool has_push_back  = detail::Has_push_back<Type, typename Type::value_type>::value;
	constexpr bool has_push_front = detail::Has_push_front<Type, typename Type::value_type>::value;

	std::size_t index = 0;
	Type container{};
	for (const auto& el : json) {
		if constexpr (has_push_back) {
			container.push_back(decode<typename Type::value_type>(el, transformer));
		} else if constexpr (has_push_front) {
			container.push_front(decode<typename Type::value_type>(el, transformer));
		} else {
			static_assert(detail::Has_subscript<Type, typename Type::value_type>::value,
			              "fixed containers need to provide subscript access");
			if (container.size() <= index) {
				throw std::system_error{ Code::too_many_entries };
			}
			container[index++] = decode<typename Type::value_type>(el, transformer);
		}
	}

	if constexpr (!has_push_back && !has_push_front) {
		if (container.size() != index) {
			throw std::system_error{ Code::too_few_entries };
		}
	}
	return container;
}

template<typename Type, typename Transformer>
inline typename std::enable_if<detail::Is_associative_container<Type>::value, Type>::type
  decode(const boost::json::value& json, const Transformer& transformer)
{
	return decode<Type>(json.as_object(), transformer);
}

template<typename Type, typename Transformer>
inline typename std::enable_if<detail::Is_associative_container<Type>::value, Type>::type
  decode(const boost::json::object& json, const Transformer& transformer)
{
	Type container;
	for (const auto& el : json) {
		container.insert(typename Type::value_type{
		  el.key_c_str(), decode<typename Type::mapped_type>(el.value(), transformer) });
	}
	return container;
}

template<typename Type, typename Transformer>
inline typename std::enable_if<detail::Is_optional<Type>::value, Type>::type
  decode(const boost::json::value& json, const Transformer& transformer)
{
	if (!json.is_null()) {
		return decode<typename Type::value_type>(json, transformer);
	}
	return std::nullopt;
}

template<typename Object, typename Transformer>
inline typename std::enable_if<boost::describe::has_describe_members<Object>::value, Object>::type
  decode(const boost::json::value& json, const Transformer& transformer)
{
	return decode<Object>(json.as_object(), transformer);
}

template<typename Object, typename Transformer>
inline typename std::enable_if<boost::describe::has_describe_members<Object>::value, Object>::type
  decode(const boost::json::object& json, const Transformer& transformer)
{
	Object value{};
	using Members = boost::describe::describe_members<Object, boost::describe::mod_any_access>;
	boost::mp11::mp_for_each<Members>([&](auto member) {
		constexpr auto name = Transformer::template normalize<Transformer::length(member.name)>(member.name);

		if (const auto it = json.find(name.c_str()); it == json.end()) {
			if (!detail::Is_optional<decltype(value.*member.pointer)>::value) {
				throw std::system_error{ Code::missing_field, name.c_str() };
			}
		} else {
			value.*member.pointer = decode<typename std::remove_reference<decltype(value.*member.pointer)>::type>(
			  it->value(), transformer);
		}
	});
	return value;
}

} // namespace smart_json
