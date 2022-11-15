#include "ErrorHandler.h"
#include <iostream>

void Error::fatal(std::string const &s) noexcept
{
	if (!s.empty())
		std::cerr << s << std::endl;
	exit(EXIT_FAILURE);
}
