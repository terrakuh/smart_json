#pragma once

#include <optional>
#include <type_traits>

namespace smart_json::detail {

template<typename Type>
struct Is_optional : std::false_type
{};

template<typename Type>
struct Is_optional<std::optional<Type>> : std::true_type
{};

} // namespace smart_json::detail
