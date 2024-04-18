#pragma once
#include<chrono>
class Timer {
public:
	Timer();
	void newFrame();
	float getDuration();
private:
	std::chrono::high_resolution_clock::time_point m_lastTime;
	float m_duration;
};