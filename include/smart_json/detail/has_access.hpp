#pragma once

#include "../error.hpp"

#include <type_traits>
#include <utility>

namespace smart_json::detail {

template<typename Container, typename Type, typename = void>
struct Has_push_front : std::false_type
{};

template<typename Container, typename Type>
struct Has_push_front<Container, Type,
                      std::void_t<decltype(std::declval<Container>().push_front(std::declval<Type>()))>>
    : std::true_type
{};

template<typename Container, typename Type, typename = void>
struct Has_push_back : std::false_type
{};

template<typename Container, typename Type>
struct Has_push_back<Container, Type,
                     std::void_t<decltype(std::declval<Container>().push_back(std::declval<Type>()))>>
    : std::true_type
{};

template<typename Container, typename Type, typename = void>
struct Has_subscript : std::false_type
{};

template<typename Container, typename Type>
struct Has_subscript<
  Container, Type,
  std::void_t<decltype(std::declval<Container>()[std::declval<std::size_t>()] = std::declval<Type>())>>
    : std::true_type
{};

} // namespace smart_json::detail
