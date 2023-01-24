#include "Clock.h"
#include "ErrorHandler.h"

using std::chrono::duration, std::chrono::nanoseconds, std::chrono::steady_clock;

void Clock::update_clock() noexcept
{
	this->current = steady_clock::now();
	if (this->first_update)
	{
		this->time_delta = std::chrono::duration<long long, std::nano>(0);
		this->first_update = false;
	}
	else
	{
		this->time_delta = this->current - this->previous;
	}
	this->previous = this->current;
}

std::chrono::duration<long long, std::nano> Clock::get_time_delta() const
{
	if (this->first_update)
	{
		Error::output_error(Error::WARNING, "get_time_delta() was called before the first update_clock() call", true);
		return std::chrono::duration<long long, std::nano>(0);
	}
	return this->time_delta;
}
