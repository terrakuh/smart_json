#pragma once

#include <system_error>
#include <type_traits>

namespace smart_json {

enum class Code
{
	success,

	unknown_enum_value,
	missing_field,
	too_many_entries,
	too_few_entries,
	bad_input_length,
};

inline const std::error_category& error_category() noexcept
{
	static class : public std::error_category
	{
	public:
		const char* name() const noexcept override
		{
			return "smart-json";
		}
		std::string message(int code) const override
		{
			switch (static_cast<Code>(code)) {
			case Code::success: return "success";

			case Code::unknown_enum_value: return "unknown enum value";
			case Code::missing_field: return "missing field";
			case Code::too_many_entries: return "too many entries in fixed array";
			case Code::too_few_entries: return "too few entries in fixed array";
			case Code::bad_input_length: return "bad input length";

			default: return "(unknown error code)";
			}
		}
	} category;
	return category;
}

inline std::error_code make_error_code(Code code) noexcept
{
	return { static_cast<int>(code), error_category() };
}

} // namespace smart_json

namespace std {

template<>
struct is_error_code_enum<smart_json::Code> : true_type
{};

} // namespace std
