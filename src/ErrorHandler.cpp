#include "ErrorHandler.h"
#include <iostream>
#include <execinfo.h>
#include <unistd.h>
#include <cstring>
#include <signal.h>

namespace
{
	void segfault_handler(int signal) noexcept
	{
		std::string output = "Error signal: " + std::to_string(signal) + "\n";
		if (std::optional<std::vector<std::string>> const stacktrace = get_stacktrace())
			for (std::string const &line : *stacktrace)
				output += line + '\n';
		exit(EXIT_FAILURE);
	}
}

std::optional<std::vector<std::string>> Error::get_stacktrace() const noexcept
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
	std::vector<std::string> output;
	output.push_back("Obtained " + std::to_string(n_symbols) + " stack frames.");
	for (size_t i = 0; i < n_symbols; i++)
		if (char_lines[i])
			output.push_back(std::string(char_lines[i]));
	free(char_lines);
	char_lines = NULL;
	return output;
}

void Error::setup_segfault_signalhandler() noexcept
{
	signal(SIGSEGV, segfault_handler);
}

void Error::output_error(
	Error::Type const type,
	std::string const &explanation_str,
	std::optional<bool> include_stacktrace,
	std::ostream &output_stream) noexcept
{
	// create string with error title
	std::string output;
	switch (type)
	{
	case FATAL:
	{
		output += "Fatal error";
		if (include_stacktrace == std::nullopt)
			include_stacktrace = true;
		break;
	}
	case WARNING:
	{
		output += "Warning";
		if (include_stacktrace == std::nullopt)
			include_stacktrace = false;
		break;
	}
	}

	// if provided, add further explanation
	if (!explanation_str.empty())
		output += ": " + explanation_str;
	if (!output.empty())
		output += (output.back() == '.') ? "\n" : ".\n";

	// check if stacktrace is required
	if (include_stacktrace == std::nullopt)
	{
		switch (type)
		{
		case FATAL:
			include_stacktrace = true;
		case WARNING:
			include_stacktrace = false;
		}
	}

	// if required, add stacktrace
	if (include_stacktrace != false)
	{
		std::optional<std::vector<std::string>> const stacktrace = get_stacktrace();
		if (stacktrace != std::nullopt)
			for (std::string const &line : *stacktrace)
				output += line + '\n';
	}

	// output to requested output stream
	if (!output.empty())
		output_stream << output;

	// if error is fatal, exit program
	if (type == FATAL)
		exit(EXIT_FAILURE);
}
