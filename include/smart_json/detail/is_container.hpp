#pragma once

#include <string>
#include <type_traits>
#include <utility>

namespace smart_json::detail {

template<typename Type, typename = void>
struct Is_container : std::false_type {};

template<typename Type>
struct Is_container<
  Type, std::void_t<decltype(std::declval<Type>().begin() != std::declval<Type>().end()),
                    typename std::enable_if<std::is_unsigned<decltype(std::declval<Type>().size())>::value &&
                                            !std::is_same<Type, std::string>::value>::type>>
    : std::true_type {};

template<typename Type, typename = void>
struct Is_associative_container : std::false_type {};

template<typename Type>
struct Is_associative_container<
  Type, std::void_t<typename std::enable_if<Is_container<Type>::value>::type,
                    typename std::enable_if<std::is_same<typename Type::key_type, std::string>::value>::type,
                    typename Type::mapped_type,
                    decltype(std::declval<Type>().insert(std::declval<typename Type::value_type>()))>>
    : std::true_type {};

} // namespace smart_json::detail
