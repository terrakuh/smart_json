#pragma once

#include "string_container.hpp"

#include <string>

namespace smart_json {

struct No_transformer {
	/// Calculates the final string length of the normalized name.
	constexpr static std::size_t length(const char* str) noexcept
	{
		return std::char_traits<char>::length(str);
	}

	/// Normalizes the input `name` and returns a zero-terminated string as an array. `Length` must be equal to
	/// length(`name`).
	template<std::size_t Length>
	constexpr static String_container<Length> normalize(const char* name)
	{
		return { name };
	}
};

} // namespace smart_json
