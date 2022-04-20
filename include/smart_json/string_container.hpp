#pragma once

#include "error.hpp"

#include <cstddef>
#include <string>

namespace smart_json {

template<std::size_t Length>
class String_container
{
public:
	constexpr String_container(const char* value)
	{
		if (std::char_traits<char>::length(value) != Length) {
			throw std::system_error{ Code::bad_input_length };
		}
		for (std::size_t i = 0; i < Length; ++i) {
			_data[i] = value[i];
		}
	}
	constexpr String_container() = default;
	constexpr std::size_t length() const noexcept
	{
		return Length;
	}
	constexpr const char* c_str() const noexcept
	{
		return _data;
	}
	constexpr char& operator[](std::size_t index) noexcept
	{
		return _data[index];
	}

private:
	char _data[Length + 1]{};
};

} // namespace smart_json
