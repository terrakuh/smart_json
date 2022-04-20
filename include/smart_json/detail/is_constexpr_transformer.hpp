#pragma once

#include <type_traits>

namespace smart_json::detail {

template<typename Type>
std::bool_constant<(T{}.f(), true)> check_constexpr_transformer(int);

template<typename>
std::false_type check_constexpr_transformer(...);

template<typename Type>
using Is_constexpr_transformer = decltype(check_constexpr_transformer<Type>(0));

} // namespace smart_json::detail
