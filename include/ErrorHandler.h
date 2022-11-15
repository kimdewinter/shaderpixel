#ifndef ERRORHANDLER_H_INCLUDED
#define ERRORHANDLER_H_INCLUDED

#include <string>

namespace Error
{
	void fatal(std::string const &s) noexcept;
}

#endif
