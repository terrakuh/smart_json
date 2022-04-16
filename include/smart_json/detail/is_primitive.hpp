#pragma once

#include <string>
#include <type_traits>

namespace smart_json::detail {

template<typename Type, typename = void>
struct Is_primitive : std::false_type
{};

template<typename Type>
struct Is_primitive<
  Type, typename std::enable_if<std::is_arithmetic<Type>::value || std::is_same<Type, const char*>::value ||
                                std::is_same<Type, std::string>::value>::type> : std::true_type
{};

} // namespace smart_json::detail
