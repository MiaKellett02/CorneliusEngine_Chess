#pragma once

//Library includes.
#include <chrono>
#include <string>

//Project includes.
#include "Logging.h"

//Declarations.
class Timer {
public:
	std::string timerName;

	Timer(std::string a_timerName) {
		m_startTimePoint = std::chrono::high_resolution_clock::now();
		timerName = a_timerName;
	}

	~Timer() {
		auto endTimepoint = std::chrono::high_resolution_clock::now();
		
		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimePoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		auto duration = end - start;
		double ms = duration * 0.001;

		std::string output = "\n" + timerName + " took " + std::to_string(ms) + "ms\n";
		//CorneliusEngine::Log(output);
		std::cout << output << std::endl;
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimePoint;
};