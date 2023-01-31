#include "ErrorHandler.h"
#include <execinfo.h>
// #include <unistd.h>
// #include <cstring>
#include <signal.h>

namespace
{
	void segfault_handler(int signal) noexcept
	{
		if (std::optional<std::string> const stacktrace = Error::get_stacktrace())
		{
			std::cerr << "Error signal: " << std::to_string(signal) << "\n"
					  << *stacktrace << std::endl;
		}
		exit(EXIT_FAILURE);
	}
}

std::optional<std::string> Error::get_stacktrace() noexcept
{
	// get the backtrace symbols
	void *symbols[30];
	memset(symbols, 0, sizeof(void *) * 30);
	size_t n_symbols = backtrace(symbols, 30);
	if (!*symbols || n_symbols == 0)
		return std::nullopt;

	// extract trace-lines from symbols as raw char arrays
	char **char_lines = backtrace_symbols(symbols, n_symbols);
	if (!char_lines)
		return std::nullopt;

	// convert std::strings for more orderly output,
	// according to GNU documentation individual strings in char_arrays need not be freed only the variable char_arrays itself
	std::string output = "Obtained " + std::to_string(n_symbols) + " stack frames.\n";
	for (size_t i = 0; i < n_symbols; i++)
		if (char_lines[i])
			output += std::string(char_lines[i]) + "\n";
	free(char_lines);
	char_lines = NULL;
	return output;
}

void Error::output_stacktrace(std::ostream &output_stream) noexcept
{
	if (std::optional<std::string> const stacktrace = Error::get_stacktrace())
		output_stream << *stacktrace << std::endl;
}

void Error::setup_segfault_signalhandler() noexcept
{
	signal(SIGSEGV, segfault_handler);
}

void Error::output_warning(std::string const &str, std::ostream &output_stream) noexcept
{
	ASSERT(!str.empty(), "empty string");
	std::cerr << "WARNING: " << str << std::endl;
}
