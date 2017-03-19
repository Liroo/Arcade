#ifndef TIME_HPP
# define TIME_HPP

# include <thread>
# include <condition_variable>
# include <mutex>
# include <future>
# include <chrono>

template<class Rep, class Period>
class Time {
  public:
    Time(std::chrono::duration<Rep, Period> duration) {
      _duration = duration;
    }
    ~Time() {}

  public:
    bool waitFor() {
      std::unique_lock<std::mutex> lock(_mutex);
      auto duration = _duration;
      _killed = false;

      return !_condition.wait_for(lock, duration, [&] {
        return _killed;
      });
    }

    void stopWaitFor() {
      std::unique_lock<std::mutex> lock(_mutex);

      _killed = true;
      _condition.notify_all();
    }

  private:
    std::condition_variable _condition;
    std::chrono::duration<Rep, Period> _duration;
    std::mutex _mutex;
    bool _killed;
};

#endif
