#pragma once

#include "detail/do_encode.inl"

namespace smart_json {

constexpr Camel_case_transformer default_encode_transformer{};

template<typename JSON, typename Type, typename Transformer = decltype(default_encode_transformer)>
inline void encode(JSON& json, const Type& value, const Transformer& transformer = default_encode_transformer)
{
	detail::do_encode(json, value, transformer);
}

template<typename JSON, typename Type, typename Transformer = decltype(default_encode_transformer)>
inline JSON encode(const Type& value, const Transformer& transformer = default_encode_transformer)
{
	JSON json{};
	encode(json, value, transformer);
	return json;
}

} // namespace smart_json
