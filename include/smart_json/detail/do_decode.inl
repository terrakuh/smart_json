#pragma once

#include "../adapter.hpp"
#include "../codec.hpp"
#include "../error.hpp"
#include "do_decode.hpp"
#include "has_access.hpp"

#include <boost/mp11.hpp>
#include <boost/numeric/conversion/cast.hpp>

namespace smart_json::detail {

template<typename JSON, typename Primitive, typename Transformer>
inline typename std::enable_if<!Has_codec<Primitive>::value && Is_primitive<Primitive>::value>::type
  do_decode(const JSON& json, Primitive& output, const Transformer& transformer)
{
	output = adapter::Adapter<JSON>::template get<Primitive>(json);
}

template<typename JSON, typename Type, typename Transformer>
inline typename std::enable_if<Has_codec<Type>::value>::type do_decode(const JSON& json, Type& output,
                                                                       const Transformer& transformer)
{
	Codec<Type>::decode(json, output);
}

template<typename JSON, typename Enum, typename Transformer>
inline typename std::enable_if<!Has_codec<Enum>::value &&
                               boost::describe::has_describe_enumerators<Enum>::value>::type
  do_decode(const JSON& json, Enum& output, const Transformer& transformer)
{
	if (auto str = adapter::Adapter<JSON>::template get<std::string>(json);
	    !boost::describe::enum_from_string(str.c_str(), output)) {
		throw std::system_error{ Code::unknown_enum_value, std::move(str) };
	}
}

template<typename JSON, typename Type, typename Transformer>
inline typename std::enable_if<!Has_codec<Type>::value && Is_container<Type>::value &&
                               !Is_associative_container<Type>::value>::type
  do_decode(const JSON& json, Type& output, const Transformer& transformer)
{
	constexpr bool has_push_back  = Has_push_back<Type, typename Type::value_type>::value;
	constexpr bool has_push_front = Has_push_front<Type, typename Type::value_type>::value;

	std::size_t index = 0;
	const auto& array = adapter::Adapter<JSON>::as_array(json);
	for (auto it = array.begin(); it != array.end(); ++it) {
		const auto& element = adapter::Adapter<JSON>::to_element(it);
		if constexpr (has_push_back) {
			typename Type::value_type value{};
			do_decode(element, value, transformer);
			output.push_back(std::move(value));
		} else if constexpr (has_push_front) {
			typename Type::value_type value{};
			do_decode(element, value, transformer);
			output.push_front(std::move(value));
		} else {
			static_assert(Has_subscript<Type, typename Type::value_type>::value,
			              "fixed containers need to provide subscript access");
			if (output.size() <= index) {
				throw std::system_error{ Code::too_many_entries };
			}
			do_decode(element, output[index++], transformer);
		}
	}

	if constexpr (!has_push_back && !has_push_front) {
		if (output.size() != index) {
			throw std::system_error{ Code::too_few_entries };
		}
	}
}

template<typename JSON, typename Type, typename Transformer>
inline typename std::enable_if<!Has_codec<Type>::value && Is_associative_container<Type>::value>::type
  do_decode(const JSON& json, Type& output, const Transformer& transformer)
{
	const auto& object = adapter::Adapter<JSON>::as_object(json);
	for (auto it = object.begin(); it != object.end(); ++it) {
		auto [key, value] = adapter::Adapter<JSON>::to_key_value(it);
		typename Type::mapped_type mapped{};
		do_decode(value, mapped, transformer);
		output.insert(typename Type::value_type{ std::move(key), std::move(mapped) });
	}
}

template<typename JSON, typename Type, typename Transformer>
inline typename std::enable_if<!Has_codec<Type>::value && Is_optional<Type>::value>::type
  do_decode(const JSON& json, Type& output, const Transformer& transformer)
{
	if (!adapter::Adapter<JSON>::is_null(json)) {
		do_decode(json, output.emplace(), transformer);
	}
}

template<typename JSON, typename Object, typename Transformer>
inline typename std::enable_if<!Has_codec<Object>::value &&
                               boost::describe::has_describe_members<Object>::value>::type
  do_decode(const JSON& json, Object& output, const Transformer& transformer)
{
	using Members      = boost::describe::describe_members<Object, boost::describe::mod_any_access>;
	const auto& object = adapter::Adapter<JSON>::as_object(json);
	boost::mp11::mp_for_each<Members>([&](auto member) {
		constexpr auto name = Transformer::template normalize<Transformer::length(member.name)>(member.name);

		if (const auto field = adapter::Adapter<JSON>::find(object, name.c_str()); field == nullptr) {
			if (!Is_optional<typename std::decay<decltype(output.*member.pointer)>::type>::value) {
				throw std::system_error{ Code::missing_field, name.c_str() };
			}
		} else {
			do_decode(*field, output.*member.pointer, transformer);
		}
	});
}

} // namespace smart_json::detail
