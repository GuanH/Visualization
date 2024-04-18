#include "Timer.h"

Timer::Timer() :m_lastTime(std::chrono::high_resolution_clock::now()), m_duration(0) {}

void Timer::newFrame() {
	auto now = std::chrono::high_resolution_clock::now();
	m_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(now - m_lastTime).count() / 1000000000.f;
	m_lastTime = now;
}

float Timer::getDuration() {
	return m_duration;
}
