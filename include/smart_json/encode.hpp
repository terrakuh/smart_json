#pragma once

#include "camel_case_transformer.hpp"
#include "detail/is_container.hpp"
#include "detail/is_primitive.hpp"

#include <boost/describe.hpp>
#include <boost/json.hpp>
#include <optional>
#include <type_traits>

namespace smart_json {

constexpr Camel_case_transformer default_encode_transformer{};

template<typename Primitive, typename Transformer = decltype(default_encode_transformer)>
typename std::enable_if<detail::Is_primitive<Primitive>::value, boost::json::value>::type
  encode(const Primitive& value, const Transformer& transformer = default_encode_transformer);

template<typename Enum, typename Transformer = decltype(default_encode_transformer)>
typename std::enable_if<boost::describe::has_describe_enumerators<Enum>::value, boost::json::string>::type
  encode(const Enum& value, const Transformer& transformer = default_encode_transformer);

template<typename Type, typename Transformer = decltype(default_encode_transformer)>
typename std::enable_if<detail::Is_container<Type>::value && !detail::Is_associative_container<Type>::value,
                        boost::json::array>::type
  encode(const Type& value, const Transformer& transformer = default_encode_transformer);

template<typename Type, typename Transformer = decltype(default_encode_transformer)>
typename std::enable_if<detail::Is_associative_container<Type>::value, boost::json::object>::type
  encode(const Type& value, const Transformer& transformer = default_encode_transformer);

template<typename Type, typename Transformer = decltype(default_encode_transformer)>
boost::json::value encode(const std::optional<Type>& value,
                          const Transformer& transformer = default_encode_transformer);

template<typename Object, typename Transformer = decltype(default_encode_transformer)>
typename std::enable_if<boost::describe::has_describe_members<Object>::value, boost::json::object>::type
  encode(const Object& value, const Transformer& transformer = default_encode_transformer);

} // namespace smart_json
