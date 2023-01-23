#include "Clock.h"

using std::chrono::duration, std::chrono::nanoseconds, std::chrono::steady_clock;

void Clock::update_clock() noexcept
{
	this->current = steady_clock::now();
	if (first_time)
	{
		this->previous = this->current;
		this->time_delta = duration<long long, std::nano>(0);
	}
	else
	{
		this->time_delta = this->current - this->previous;
		this->previous = this->current;
	}

	this->current = steady_clock::now();
	if (!this->first_time)
	{
		this->time_delta = this->current - this->previous;
	}
	this->previous = this->current;
}

std::chrono::duration<long long, std::nano> Clock::get_time_delta() const noexcept
{
	return this->time_delta;
}
