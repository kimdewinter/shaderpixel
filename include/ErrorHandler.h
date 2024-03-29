#ifndef ERRORHANDLER_H_INCLUDED
#define ERRORHANDLER_H_INCLUDED

#include <optional>
#include <iostream>

#ifndef NDEBUG
/// @brief if 'bool_condition' is false, prints 'message' and stacktrace to cerr, then terminates program
#define ASSERT(bool_condition, message)                                                \
	do                                                                                 \
	{                                                                                  \
		if (!(bool_condition))                                                         \
		{                                                                              \
			std::cerr << "Assertion '" #bool_condition "' failed in " << __FILE__      \
					  << " line " << __LINE__ << ": " << message << std::endl;         \
			if (std::optional<std::string> const stacktrace = Error::get_stacktrace()) \
				std::cerr << *stacktrace << std::endl;                                 \
			std::terminate();                                                          \
		}                                                                              \
	} while (false)
#else
/// @brief if 'bool_condition' is false, prints 'message' to cerr
#define ASSERT(bool_condition, message)                                           \
	do                                                                            \
	{                                                                             \
		if (!(bool_condition))                                                    \
		{                                                                         \
			std::cerr << "Assertion '" #bool_condition "' failed in " << __FILE__ \
					  << " line " << __LINE__ << ": " << message << std::endl;    \
		}                                                                         \
	} while (false)
#endif

/// @brief if 'bool_condition' is false, prints 'message' to cerr
#define SOFT_ASSERT(bool_condition, message)                                           \
	do                                                                                 \
	{                                                                                  \
		if (!(bool_condition))                                                         \
		{                                                                              \
			std::cerr << "Soft assertion '" #bool_condition "' failed in " << __FILE__ \
					  << " line " << __LINE__ << ": " << message << std::endl;         \
		}                                                                              \
	} while (false)

namespace Error
{
	std::optional<std::string> get_stacktrace() noexcept;

	/// @param output_stream where to output the stacktrace to; default is std::cerr
	void output_stacktrace(std::ostream &output_stream = std::cerr) noexcept;

	/// @brief prints stack trace upon segfault; to use, simply call at the very beginning of main
	void setup_segfault_signalhandler() noexcept;
}

#endif
