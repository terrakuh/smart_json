#pragma once

#include "camel_case_transformer.hpp"
#include "detail/is_container.hpp"
#include "detail/is_optional.hpp"
#include "detail/is_primitive.hpp"

#include <boost/describe.hpp>
#include <boost/json.hpp>
#include <optional>
#include <type_traits>

namespace smart_json {

constexpr Camel_case_transformer default_decode_transformer{};

template<typename Primitive, typename Transformer = decltype(default_decode_transformer)>
typename std::enable_if<detail::Is_primitive<Primitive>::value, Primitive>::type
  decode(const boost::json::value& json, const Transformer& transformer = default_decode_transformer);

template<typename Enum, typename Transformer = decltype(default_decode_transformer)>
typename std::enable_if<boost::describe::has_describe_enumerators<Enum>::value, Enum>::type
  decode(const boost::json::value& json, const Transformer& transformer = default_decode_transformer);

template<typename Enum, typename Transformer = decltype(default_decode_transformer)>
typename std::enable_if<boost::describe::has_describe_enumerators<Enum>::value, Enum>::type
  decode(const boost::json::string& json, const Transformer& transformer = default_decode_transformer);

template<typename Type, typename Transformer = decltype(default_decode_transformer)>
typename std::enable_if<detail::Is_container<Type>::value && !detail::Is_associative_container<Type>::value,
                        Type>::type
  decode(const boost::json::value& json, const Transformer& transformer = default_decode_transformer);

template<typename Type, typename Transformer = decltype(default_decode_transformer)>
typename std::enable_if<detail::Is_container<Type>::value && !detail::Is_associative_container<Type>::value,
                        Type>::type
  decode(const boost::json::array& json, const Transformer& transformer = default_decode_transformer);

template<typename Type, typename Transformer = decltype(default_decode_transformer)>
typename std::enable_if<detail::Is_associative_container<Type>::value, Type>::type
  decode(const boost::json::value& json, const Transformer& transformer = default_decode_transformer);

template<typename Type, typename Transformer = decltype(default_decode_transformer)>
typename std::enable_if<detail::Is_associative_container<Type>::value, Type>::type
  decode(const boost::json::object& json, const Transformer& transformer = default_decode_transformer);

template<typename Type, typename Transformer = decltype(default_decode_transformer)>
typename std::enable_if<detail::Is_optional<Type>::value, Type>::type
  decode(const boost::json::value& json, const Transformer& transformer = default_decode_transformer);

template<typename Object, typename Transformer = decltype(default_decode_transformer)>
typename std::enable_if<boost::describe::has_describe_members<Object>::value, Object>::type
  decode(const boost::json::value& json, const Transformer& transformer = default_decode_transformer);

template<typename Object, typename Transformer = decltype(default_decode_transformer)>
typename std::enable_if<boost::describe::has_describe_members<Object>::value, Object>::type
  decode(const boost::json::object& json, const Transformer& transformer = default_decode_transformer);

} // namespace smart_json
