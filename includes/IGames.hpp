#ifndef IGAMES_HPP
# define IGAMES_HPP

# include <string>
# include <chrono>

# include "Event.hpp"
# include "Object.hpp"

namespace Arcade {

  class IGames {
  public:
    virtual ~IGames() {};

  public:
    /*
      start:
      function for init the game
    */
    virtual void start() = 0;

  public:
    /*
      tick:
      return vector of object each tick of times for update game
    */
    virtual std::vector<Arcade::Object> tick(std::chrono::milliseconds const &time) = 0;
    /*
      eventHandling:
      handle event from graphic parts, this could be a key or resize but resize is not handled by games
    */
    virtual std::vector<Arcade::Object> eventHandling(const Event&) = 0;
    /*
      dump:
      return vector of all objects
    */
    virtual std::vector<Arcade::Object> dump() = 0;

    };
}

#endif
