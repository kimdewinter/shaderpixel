#include "ErrorHandler.h"
#include <iostream>
#include <execinfo.h>
#include <unistd.h>

namespace
{
	void segfault_handler(int signal) noexcept
	{
		void *array[30];
		size_t size = backtrace(array, 30);
		std::cerr << "Error: signal " << signal << ":" << std::endl;
		backtrace_symbols_fd(array, size, STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
}

void Error::setup_segfault_signalhandler() noexcept
{
	signal(SIGSEGV, segfault_handler);
}

void Error::fatal(std::string const &s) noexcept
{
	if (!s.empty())
		std::cerr << s << std::endl;
	exit(EXIT_FAILURE);
}
