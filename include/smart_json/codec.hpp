#pragma once

#include "adapter.hpp"

#include <chrono>

namespace smart_json {

template<typename Type, typename = void>
struct Codec {
	typedef void Undefined;
};

template<typename Clock, typename Duration>
struct Codec<std::chrono::time_point<Clock, Duration>, void> {
	template<typename JSON>
	static void encode(JSON& json, const std::chrono::time_point<Clock, Duration>& value)
	{
		adapter::Adapter<JSON>::put(json, value.time_since_epoch().count());
	}
	template<typename JSON>
	static auto decode(const JSON& json, std::chrono::time_point<Clock, Duration>& value)
	{
		value = std::chrono::time_point<Clock, Duration>{ Duration{
			adapter::Adapter<JSON>::template get<typename Duration::rep>(json) } };
	}
};

} // namespace smart_json
