#pragma once

#include "detail/is_container.hpp"
#include "detail/is_primitive.hpp"

#include <boost/describe.hpp>
#include <boost/json.hpp>
#include <optional>
#include <type_traits>

namespace smart_json {

template<typename Primitive>
typename std::enable_if<detail::Is_primitive<Primitive>::value>::type encode(const Primitive& value,
                                                                             boost::json::value& json);

template<typename Enum>
typename std::enable_if<boost::describe::has_describe_enumerators<Enum>::value>::type
  encode(const Enum& value, boost::json::value& json);

template<typename Type>
typename std::enable_if<detail::Is_container<Type>::value &&
                        !detail::Is_associative_container<Type>::value>::type
  encode(const Type& value, boost::json::value& json);

template<typename Type>
typename std::enable_if<detail::Is_associative_container<Type>::value>::type encode(const Type& value,
                                                                                    boost::json::value& json);

template<typename Type>
void encode(const std::optional<Type>& value, boost::json::value& json);

template<typename Object>
typename std::enable_if<boost::describe::has_describe_members<Object>::value>::type
  encode(const Object& value, boost::json::value& json);

} // namespace smart_json
