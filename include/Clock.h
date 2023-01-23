#ifndef CLOCK__H__
#define CLOCK__H__

#include <chrono>

using std::chrono::time_point, std::chrono::steady_clock, std::chrono::duration;

class Clock
{
public:
	void update_clock() noexcept;
	std::chrono::duration<long long, std::nano> get_time_delta() const noexcept;

private:
	bool first_time = false; // necessary because "previous" won't have a valid value on the first loop
	time_point<steady_clock> previous;
	time_point<steady_clock> current;
	std::chrono::duration<long long, std::nano> time_delta =
		std::chrono::duration<long long, std::nano>(0); // time elapsed between previous and current
};

#endif