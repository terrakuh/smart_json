#pragma once

namespace smart_json::adapter {

template<typename Type, typename = void>
struct Adapter;

}

#include "adapter/boost_json.hpp"
#include "adapter/boost_property_tree.hpp"

#if __has_include(<nlohmann/json.hpp>)
#include "adapter/nlohmann_json.hpp"
#endif
