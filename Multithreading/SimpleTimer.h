#pragma once

#include <iostream>
#include <chrono>

class SimpleTimer {
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
	std::chrono::duration<float> duration;

public:
	SimpleTimer();
	~SimpleTimer();

};


