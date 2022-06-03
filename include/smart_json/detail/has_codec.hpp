#pragma once

#include "../codec.hpp"

#include <type_traits>

namespace smart_json::detail {

template<typename Type, typename = void>
struct Has_codec : std::true_type {};

template<typename Type>
struct Has_codec<Type, std::void_t<typename Codec<Type>::Undefined>>
    : std::false_type {};

} // namespace smart_json::detail
