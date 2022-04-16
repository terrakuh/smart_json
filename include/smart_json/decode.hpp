#pragma once

#include "detail/is_container.hpp"
#include "detail/is_optional.hpp"
#include "detail/is_primitive.hpp"

#include <boost/describe.hpp>
#include <boost/json.hpp>
#include <optional>
#include <type_traits>

namespace smart_json {

template<typename Primitive>
typename std::enable_if<detail::Is_primitive<Primitive>::value, Primitive>::type
  decode(const boost::json::value& json);

template<typename Enum>
typename std::enable_if<boost::describe::has_describe_enumerators<Enum>::value, Enum>::type
  decode(const boost::json::value& json);

template<typename Enum>
typename std::enable_if<boost::describe::has_describe_enumerators<Enum>::value, Enum>::type
  decode(const boost::json::string& json);

template<typename Type>
typename std::enable_if<detail::Is_container<Type>::value && !detail::Is_associative_container<Type>::value,
                        Type>::type
  decode(const boost::json::value& json);

template<typename Type>
typename std::enable_if<detail::Is_container<Type>::value && !detail::Is_associative_container<Type>::value,
                        Type>::type
  decode(const boost::json::array& json);

template<typename Type>
typename std::enable_if<detail::Is_associative_container<Type>::value, Type>::type
  decode(const boost::json::value& json);

template<typename Type>
typename std::enable_if<detail::Is_associative_container<Type>::value, Type>::type
  decode(const boost::json::object& json);

template<typename Type>
typename std::enable_if<detail::Is_optional<Type>::value, Type>::type decode(const boost::json::value& json);

template<typename Object>
typename std::enable_if<boost::describe::has_describe_members<Object>::value, Object>::type
  decode(const boost::json::value& json);

template<typename Object>
typename std::enable_if<boost::describe::has_describe_members<Object>::value, Object>::type
  decode(const boost::json::object& json);

} // namespace smart_json
