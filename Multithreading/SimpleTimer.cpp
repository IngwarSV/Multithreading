#include "SimpleTimer.h"

SimpleTimer::SimpleTimer() {
	this->start = std::chrono::high_resolution_clock::now();
	this->duration = std::chrono::high_resolution_clock::duration::zero();
}

SimpleTimer::~SimpleTimer() {
	this->end = std::chrono::high_resolution_clock::now();
	this->duration = this->end - this->start;
	float result = this->duration.count();

	std::cout << "Time passed: " << result << "seconds" << std::endl;
}
