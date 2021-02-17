#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer{
public:
	Timer();

	void reset();
	const double elapsed() const;

	~Timer();

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> begin;
};

#endif