#include "timer.h"

Timer::Timer() : begin(std::chrono::high_resolution_clock::now()){
}

void Timer::reset(){
	begin = std::chrono::high_resolution_clock::now();
}

const double Timer::elapsed() const{
	return std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1>>>(std::chrono::high_resolution_clock::now() - begin).count();
}

Timer::~Timer() = default;
