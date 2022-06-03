#pragma once

#include "has_codec.hpp"
#include "is_container.hpp"
#include "is_optional.hpp"
#include "is_primitive.hpp"

#include <boost/describe.hpp>
#include <boost/json.hpp>
#include <optional>
#include <type_traits>

namespace smart_json::detail {

template<typename JSON, typename Primitive, typename Transformer>
typename std::enable_if<Is_primitive<Primitive>::value>::type do_decode(const JSON& json, Primitive& output,
                                                                        const Transformer& transformer);

template<typename JSON, typename Type, typename Transformer>
typename std::enable_if<Has_codec<Type>::value>::type do_decode(const JSON& json, Type& output,
                                                                const Transformer& transformer);

template<typename JSON, typename Enum, typename Transformer>
typename std::enable_if<boost::describe::has_describe_enumerators<Enum>::value>::type
  do_decode(const JSON& json, Enum& output, const Transformer& transformer);

template<typename JSON, typename Type, typename Transformer>
typename std::enable_if<Is_container<Type>::value && !Is_associative_container<Type>::value>::type
  do_decode(const JSON& json, Type& output, const Transformer& transformer);

template<typename JSON, typename Type, typename Transformer>
typename std::enable_if<Is_associative_container<Type>::value>::type
  do_decode(const JSON& json, Type& output, const Transformer& transformer);

template<typename JSON, typename Type, typename Transformer>
typename std::enable_if<Is_optional<Type>::value>::type do_decode(const JSON& json, Type& output,
                                                                  const Transformer& transformer);

template<typename JSON, typename Object, typename Transformer>
typename std::enable_if<boost::describe::has_describe_members<Object>::value>::type
  do_decode(const JSON& json, Object& output, const Transformer& transformer);

} // namespace smart_json::detail
