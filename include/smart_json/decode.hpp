#pragma once

#include "detail/do_decode.inl"

namespace smart_json {

constexpr Camel_case_transformer default_decode_transformer{};

template<typename JSON, typename Type, typename Transformer = decltype(default_decode_transformer)>
inline void decode(const JSON& json, Type& value, const Transformer& transformer = default_decode_transformer)
{
	detail::do_decode(json, value, transformer);
}

template<typename Type, typename JSON, typename Transformer = decltype(default_decode_transformer)>
inline Type decode(const JSON& json, const Transformer& transformer = default_decode_transformer)
{
	Type value{};
	decode(json, value, transformer);
	return value;
}

} // namespace smart_json
