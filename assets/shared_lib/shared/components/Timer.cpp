#include "Timer.h"

Timer::Timer() {
	_paused = false;
	_started = false;
}

Timer::~Timer() {}

bool Timer::isStarted() {
	return _started;
}
bool Timer::isStopped() {
	return !_started;
}
bool Timer::isPaused() {
	return _paused;
}
bool Timer::isActive() {
	return !_paused & _started;
}
void Timer::pause() {
	if (_paused || !_started) {
		return;
  }
	_paused = true;
	_pausedAt = std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::system_clock::now().time_since_epoch());
}
void Timer::resume() {
	if (!_paused) {
		return;
  }
	_paused = false;
	_startedAt += std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::system_clock::now().time_since_epoch()) - _pausedAt;
}
void Timer::stop() {
	_started = false;
}
void Timer::start() {
	if (_started) {
		return;
  }
	_started = true;
	_paused = false;
	_startedAt = std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::system_clock::now().time_since_epoch());
}
void Timer::reset() {
	_paused = false;
	_startedAt = std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::system_clock::now().time_since_epoch());
}
long Timer::getTick() {
	if (!_started) {
		return 0;
  }
	if (_paused) {
    return (_pausedAt - _startedAt).count();
  }
	return (std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::system_clock::now().time_since_epoch()) - _startedAt).count();
}
