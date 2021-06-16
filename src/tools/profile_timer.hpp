#pragma once

#include <chrono>
#include <string>

class ProfileTimer {
public:
	typedef std::chrono::high_resolution_clock Clock;

	ProfileTimer(std::string const& name);
	~ProfileTimer();

	void Stop();

private:
	std::string name;
	Clock::time_point startTime;
	bool stopped = false;
};