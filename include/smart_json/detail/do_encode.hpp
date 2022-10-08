#pragma once

#include "is_container.hpp"
#include "is_primitive.hpp"

#include <boost/describe.hpp>
#include <boost/json.hpp>
#include <optional>
#include <type_traits>

namespace smart_json::detail {

template<typename JSON, typename Primitive, typename Transformer>
typename std::enable_if<!Has_codec<Primitive>::value && Is_primitive<Primitive>::value, bool>::type
  do_encode(JSON& json, const Primitive& value, const Transformer& transformer);

template<typename JSON, typename Type, typename Transformer>
typename std::enable_if<Has_codec<Type>::value, bool>::type do_encode(JSON& json, const Type& value,
                                                                      const Transformer& transformer);

template<typename JSON, typename Enum, typename Transformer>
typename std::enable_if<!Has_codec<Enum>::value && boost::describe::has_describe_enumerators<Enum>::value, bool>::type
  do_encode(JSON& json, const Enum& value, const Transformer& transformer);

template<typename JSON, typename Type, typename Transformer>
typename std::enable_if<!Has_codec<Type>::value && Is_container<Type>::value && !Is_associative_container<Type>::value, bool>::type
  do_encode(JSON& json, const Type& value, const Transformer& transformer);

template<typename JSON, typename Type, typename Transformer>
typename std::enable_if<!Has_codec<Type>::value && Is_associative_container<Type>::value, bool>::type
  do_encode(JSON& json, const Type& value, const Transformer& transformer);

template<typename JSON, typename Type, typename Transformer>
bool do_encode(JSON& json, const std::optional<Type>& value, const Transformer& transformer);

template<typename JSON, typename Object, typename Transformer>
typename std::enable_if<!Has_codec<Object>::value && boost::describe::has_describe_members<Object>::value, bool>::type
  do_encode(JSON& json, const Object& value, const Transformer& transformer);

} // namespace smart_json::detail
