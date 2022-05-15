#pragma once

#include "error.hpp"
#include "string_container.hpp"

#include <string>

namespace smart_json {

struct Camel_case_transformer {
	/// Calculates the final string length of the normalized name.
	constexpr static std::size_t length(const char* str) noexcept
	{
		const std::size_t length = std::char_traits<char>::length(str);
		std::size_t count        = 0;
		for (; *str != 0; ++str) {
			if (*str == '_') {
				++count;
			}
		}
		return length - count;
	}

	/// Normalizes the input `name` and returns a zero-terminated string as an array. `Length` must be equal to
	/// length(`name`).
	template<std::size_t Length>
	constexpr static String_container<Length> normalize(const char* name)
	{
		String_container<Length> str;
		bool make_uppercase = false;
		for (std::size_t i = 0, j = 0; name[j] != 0; ++j) {
			if (name[j] == '_') {
				make_uppercase = true;
				continue;
			} else if (i >= Length) {
				throw std::system_error{ Code::bad_input_length };
			} else if (make_uppercase) {
				const char c   = name[j];
				str[i++]       = c >= 'a' && c <= 'z' ? c - 'a' + 'A' : c;
				make_uppercase = false;
			} else {
				str[i++] = name[j];
			}
		}
		return str;
	}
};

} // namespace smart_json
