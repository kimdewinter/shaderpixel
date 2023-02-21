#ifndef CLOCK__H__
#define CLOCK__H__

#include <chrono>

class Clock
{
public:
	// bunch of templates and usings to reduce clutter
	template <typename T = long long, typename E = std::nano>
	using duration = std::chrono::duration<T, E>;
	template <typename T = std::chrono::steady_clock>
	using time_point = std::chrono::time_point<T>;

	void update_clock() noexcept;
	duration<> get_time_delta() const noexcept;

private:
	bool first_update = true; // necessary because "previous" won't have a valid value on the first loop
	time_point<> previous;
	time_point<> current;
	duration<> time_delta = duration<>(0); // time elapsed between previous and current
};

#endif