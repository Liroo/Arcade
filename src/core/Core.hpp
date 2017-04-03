#ifndef CORE_HPP
# define CORE_HPP

# include <string>
# include <chrono>
# include <vector>
# include "Time.hpp"
# include "DirectoryLib.hpp"

# define ERR_STARTEDLIB "This lib is not compatible or does not exist: "

# define TICK_MS 10

namespace Arcade {
  class Core {
  public:
    Core();
    ~Core();

  private:
    DirectoryLib::DirectoryLibContent _availableLib;
    unsigned int _availableLibIndex = -1;
    DirectoryLib::DirectoryLibContent _availableGame;
    unsigned int _availableGameIndex = -1;

  public:
    bool init(const std::string &);
    int run();

  private:
    bool _isRunning = true;
    Time<float, std::milli> _timer;
  };
};

#endif
