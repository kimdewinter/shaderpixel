#include "Clock.h"
#include "ErrorHandler.h"

template <typename T = long long, typename E = std::nano>
using duration = std::chrono::duration<T, E>;
template <typename T = std::chrono::steady_clock>
using time_point = std::chrono::time_point<T>;
using std::chrono::steady_clock;

void Clock::update_clock() noexcept
{
	this->current = steady_clock::now();
	if (this->first_update)
	{
		this->time_delta = duration<>(0);
		this->first_update = false;
	}
	else
	{
		this->time_delta = this->current - this->previous;
	}
	this->previous = this->current;
}

duration<> Clock::get_time_delta() const noexcept
{
	if (this->first_update)
	{
		ASSERT(false, "get_time_delta() was called before the first update_clock() call");
		return duration<>(0);
	}
	return this->time_delta;
}
