#include "profile_timer.hpp"

#include <iostream>

ProfileTimer::ProfileTimer(std::string const& name) :
	name(name)
{
	startTime = Clock::now();
}

ProfileTimer::~ProfileTimer() {
	Stop();
}

void ProfileTimer::Stop() {
	Clock::time_point stopTime = Clock::now();

	Clock::duration duration = stopTime - startTime;

	std::cout << name << ": " << std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() << "ns" << std::endl;
}