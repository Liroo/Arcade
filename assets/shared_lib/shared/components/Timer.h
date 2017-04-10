#ifndef TIMER_H
# define TIMER_H

# include <chrono>

class Timer {
  public:
    Timer();
    ~Timer();

  private:
    std::chrono::milliseconds _startedAt;
    std::chrono::milliseconds _pausedAt;
    bool _started;
    bool _paused;

  public:
    void start();
    long getTick();
    void pause();
    void resume();
    void stop();
    void reset();

    bool isActive();
    bool isStarted();
    bool isPaused();
  	bool isStopped();
};

#endif
