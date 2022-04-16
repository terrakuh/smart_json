#pragma once

#include "decode.hpp"
#include "detail/has_access.hpp"
#include "detail/is_optional.hpp"
#include "error.hpp"

#include <boost/mp11.hpp>
#include <boost/numeric/conversion/cast.hpp>

namespace smart_json {

template<typename Primitive>
inline typename std::enable_if<detail::Is_primitive<Primitive>::value>::type
  decode(Primitive& value, const boost::json::value& json)
{
	if constexpr (std::is_same<Primitive, bool>::value) {
		value = json.as_bool();
	} else if constexpr (std::is_signed<Primitive>::value) {
		value = boost::numeric_cast<Primitive>(json.as_int64());
	} else if constexpr (std::is_unsigned<Primitive>::value) {
		value = boost::numeric_cast<Primitive>(json.as_uint64());
	} else if constexpr (std::is_floating_point<Primitive>::value) {
		value = boost::numeric_cast<Primitive>(json.as_double());
	} else {
		const auto& str = json.as_string();
		if constexpr (std::is_same<Primitive, std::string>::value) {
			value = std::string{ str.c_str(), str.size() };
		} else {
			value = str.c_str();
		}
	}
}

template<typename Enum>
inline typename std::enable_if<boost::describe::has_describe_enumerators<Enum>::value>::type
  decode(Enum& value, const boost::json::value& json)
{
	const auto& name = json.as_string();
	if (!boost::describe::enum_from_string(name.c_str(), value)) {
		throw std::system_error{ Code::unknown_enum_value, name.c_str() };
	}
}

template<typename Type>
inline typename std::enable_if<detail::Is_container<Type>::value &&
                               !detail::Is_associative_container<Type>::value>::type
  decode(Type& value, const boost::json::value& json)
{
	constexpr bool has_push_back  = detail::Has_push_back<Type, typename Type::value_type>::value;
	constexpr bool has_push_front = detail::Has_push_front<Type, typename Type::value_type>::value;

	std::size_t index = 0;
	const auto& array = json.as_array();
	for (const auto& el : array) {
		typename Type::value_type val{};
		decode(val, el);

		if constexpr (has_push_back) {
			value.push_back(std::move(val));
		} else if constexpr (has_push_front) {
			value.push_front(std::move(val));
		} else {
			static_assert(detail::Has_subscript<Type, typename Type::value_type>::value,
			              "fixed containers need to provide subscript access");
			if (value.size() <= index) {
				throw std::system_error{ Code::too_many_entries };
			}
			value[index++] = std::move(val);
		}
	}

	if constexpr (!has_push_back && !has_push_front) {
		if (value.size() != index) {
			throw std::system_error{ Code::too_few_entries };
		}
	}
}

template<typename Type>
inline typename std::enable_if<detail::Is_associative_container<Type>::value>::type
  decode(Type& value, const boost::json::value& json)
{
	const auto& object = json.as_object();
	for (const auto& el : object) {
		typename Type::mapped_type val{};
		decode(val, el.value());
		value.insert(typename Type::value_type{ el.key_c_str(), std::move(val) });
	}
}

template<typename Type>
inline void decode(std::optional<Type>& value, const boost::json::value& json)
{
	if (!json.is_null()) {
		Type val{};
		decode(val, json);
		value = std::move(val);
	}
}

template<typename Object>
inline typename std::enable_if<boost::describe::has_describe_members<Object>::value>::type
  decode(Object& value, const boost::json::value& json)
{
	const auto& object = json.as_object();
	using Members      = boost::describe::describe_members<Object, boost::describe::mod_any_access>;
	boost::mp11::mp_for_each<Members>([&](auto member) {
		if (const auto it = object.find(member.name); it == object.end()) {
			if (!detail::Is_optional<decltype(value.*member.pointer)>::value) {
				throw std::system_error{ Code::missing_field, member.name };
			}
		} else {
			decode(value.*member.pointer, it->value());
		}
	});
}

} // namespace smart_json
