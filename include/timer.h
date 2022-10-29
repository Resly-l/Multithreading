#pragma once
#include <chrono>

using Seconds = std::chrono::duration<double, std::ratio<1, 1>>;
using Milliseconds = std::chrono::duration<double, std::ratio<1, 1000>>;
using Microseconds = std::chrono::duration<double, std::ratio<1, 1000000>>;
using Nanoseconds = std::chrono::duration<double, std::ratio<1, 1000000000>>;

template <typename Unit>
class Timer
{
private:
	std::chrono::steady_clock::time_point timePoint;

public:
	Timer() { timePoint = std::chrono::steady_clock::now(); }
	~Timer() {}

public:
	double Mark()
	{
		const auto now = std::chrono::steady_clock::now();
		const auto delta = std::chrono::duration_cast<Unit>(now - timePoint);

		timePoint = now;
		return delta.count();
	}

	double Peek() const
	{
		return std::chrono::duration_cast<Unit>(std::chrono::steady_clock::now() - timePoint).count();
	}
};