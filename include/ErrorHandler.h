#ifndef ERRORHANDLER_H_INCLUDED
#define ERRORHANDLER_H_INCLUDED

#include <string>

namespace Error
{
	/// @brief Prints stack trace upon segfault; to use, call at beginning of main
	void setup_segfault_signalhandler() noexcept;
	void fatal(std::string const &s) noexcept;
}

#endif
