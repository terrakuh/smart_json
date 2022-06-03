#pragma once

#include "do_encode.hpp"

#include <boost/mp11.hpp>

namespace smart_json::detail {

template<typename JSON, typename Primitive, typename Transformer>
inline typename std::enable_if<Is_primitive<Primitive>::value, bool>::type
  do_encode(JSON& json, const Primitive& primitive, const Transformer& transformer)
{
	adapter::Adapter<JSON>::put(json, primitive);
	return true;
}

template<typename JSON, typename Type, typename Transformer>
inline typename std::enable_if<Has_codec<Type>::value, bool>::type do_encode(JSON& json, const Type& value,
                                                                             const Transformer& transformer)
{
	Codec<Type>::encode(json, value);
	return true;
}

template<typename JSON, typename Enum, typename Transformer>
inline typename std::enable_if<boost::describe::has_describe_enumerators<Enum>::value, bool>::type
  do_encode(JSON& json, const Enum& enum_, const Transformer& transformer)
{
	const char* str = boost::describe::enum_to_string(enum_, nullptr);
	if (str == nullptr) {
		throw std::system_error{ Code::unknown_enum_value };
	}
	adapter::Adapter<JSON>::put(json, str);
	return true;
}

template<typename JSON, typename Type, typename Transformer>
inline
  typename std::enable_if<Is_container<Type>::value && !Is_associative_container<Type>::value, bool>::type
  do_encode(JSON& json, const Type& value, const Transformer& transformer)
{
	if (value.begin() == value.end()) {
		adapter::Adapter<JSON>::emplace_array(json);
	} else {
		for (const auto& el : value) {
			auto element = adapter::Adapter<JSON>::make_element();
			do_encode(element, el, transformer);
			adapter::Adapter<JSON>::push(json, std::move(element));
		}
	}
	return true;
}

template<typename JSON, typename Type, typename Transformer>
inline typename std::enable_if<Is_associative_container<Type>::value, bool>::type
  do_encode(JSON& json, const Type& value, const Transformer& transformer)
{
	if (value.begin() == value.end()) {
		adapter::Adapter<JSON>::emplace_object(json);
	} else {
		for (const auto& [key, el] : value) {
			auto mapped = adapter::Adapter<JSON>::make_mapped();
			do_encode(mapped, el, transformer);
			adapter::Adapter<JSON>::insert(json, key, std::move(mapped));
		}
	}
	return true;
}

template<typename JSON, typename Type, typename Transformer>
inline bool do_encode(JSON& json, const std::optional<Type>& value, const Transformer& transformer)
{
	if (value.has_value()) {
		return do_encode(json, value.value(), transformer);
	}
	return false;
}

template<typename JSON, typename Object, typename Transformer>
inline typename std::enable_if<boost::describe::has_describe_members<Object>::value, bool>::type
  do_encode(JSON& json, const Object& value, const Transformer& transformer)
{
	using Members = boost::describe::describe_members<Object, boost::describe::mod_any_access>;
	boost::mp11::mp_for_each<Members>([&](auto member) {
		constexpr auto name = Transformer::template normalize<Transformer::length(member.name)>(member.name);

		if (auto mapped = adapter::Adapter<JSON>::make_mapped();
		    do_encode(mapped, value.*member.pointer, transformer)) {
			adapter::Adapter<JSON>::insert(json, name.c_str(), std::move(mapped));
		}
	});
	return true;
}

} // namespace smart_json::detail
