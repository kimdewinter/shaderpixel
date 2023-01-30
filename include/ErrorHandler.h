#ifndef ERRORHANDLER_H_INCLUDED
#define ERRORHANDLER_H_INCLUDED

#include <optional>
#include <vector>
#include <iostream>

#ifndef NDEBUG
#define ASSERT(bool_condition, message)                                           \
	do                                                                            \
	{                                                                             \
		if (!(bool_condition))                                                    \
		{                                                                         \
			std::cerr << "Assertion '" #bool_condition "' failed in " << __FILE__ \
					  << " line " << __LINE__ << ": " << message << std::endl;    \
			Error::get_stacktrace();                                              \
			std::terminate();                                                     \
		}
}
while (false)
#else
#define ASSERT(bool_condition, message)                                       \
	do                                                                        \
	{                                                                         \
		std::cerr << "Assertion '" #bool_condition "' failed in " << __FILE__ \
				  << " line " << __Line__ << ": " << message << std::endl;    \
	} while (false)
#endif

	namespace Error
	{
		enum Type
		{
			FATAL,
			WARNING
		};

		/// @return a stacktrace, with each line in it's own separate string
		std::optional<std::vector<std::string>> get_stacktrace() const noexcept;

		/// @brief prints stack trace upon segfault; to use, call at beginning of main
		void setup_segfault_signalhandler() noexcept;

		/// @brief outputs error to requested output stream
		/// @param type type of error; used to determine brief error text & functionality; note that FATAL exits the program
		/// @param explanation_str further custom explanation of the error
		/// @param include_stacktrace whether to print an error trace
		/// @param output_stream what ostream to output to
		void output_error(
			Error::Type const type,
			std::string const &explanation_str = {},
			std::optional<bool> include_stacktrace = std::nullopt,
			std::ostream &output_stream = std::cerr) noexcept;
	}

#endif
