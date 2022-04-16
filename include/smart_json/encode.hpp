#pragma once

#include "detail/is_container.hpp"
#include "detail/is_primitive.hpp"

#include <boost/describe.hpp>
#include <boost/json.hpp>
#include <optional>
#include <type_traits>

namespace smart_json {

template<typename Primitive>
typename std::enable_if<detail::Is_primitive<Primitive>::value, boost::json::value>::type
  encode(const Primitive& value);

template<typename Enum>
typename std::enable_if<boost::describe::has_describe_enumerators<Enum>::value, boost::json::string>::type
  encode(const Enum& value);

template<typename Type>
typename std::enable_if<detail::Is_container<Type>::value && !detail::Is_associative_container<Type>::value,
                        boost::json::array>::type
  encode(const Type& value);

template<typename Type>
typename std::enable_if<detail::Is_associative_container<Type>::value, boost::json::object>::type
  encode(const Type& value);

template<typename Type>
boost::json::value encode(const std::optional<Type>& value);

template<typename Object>
typename std::enable_if<boost::describe::has_describe_members<Object>::value, boost::json::object>::type
  encode(const Object& value);

} // namespace smart_json
