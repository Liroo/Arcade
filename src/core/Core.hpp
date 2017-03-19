#ifndef CORE_HPP
# define CORE_HPP

# include <string>
# include <chrono>
# include "Time.hpp"

namespace Arcade {
  class Core {
  public:
    Core(const std::string &libName);
    ~Core();

  public:
    int run();

  private:
    std::string _libName;
    bool _isRunning = true;
    Time<float, std::milli> _timer;
  };
};

#endif
