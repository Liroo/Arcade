#ifndef CORE_HPP
# define CORE_HPP

# include <string>
# include <chrono>
# include <vector>
# include "Time.hpp"

// Defined error

# define ERR_READDIR "There is an error during reading of directory: "
# define ERR_STARTEDLIB "This lib is not compatible or does not exist: "

// Core stuff

# define TICK_MS 10
# define DIR_LIB "lib/"
# define DIR_GAMES "games/"

namespace Arcade {
  class Core {
  public:
    Core();
    ~Core();

  private:
    bool updateDirectoryContent(const char*,
      std::vector<std::string> &);

    std::vector<std::string> _availableLib;
    unsigned int _availableLibIndex = -1;
    std::vector<std::string> _availableGame;
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
