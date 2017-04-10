#ifndef AGAMES_H
# define AGAMES_H

# include <string>
# include <chrono>
# include <map>

# include "components/Timer.h"
# include "Event.h"
# include "Object.h"

namespace Arcade {
  class AGames {
  public:
    virtual ~AGames() {};

  public:
    /*
      start:
      function for init and start the game
    */
    virtual ObjectList start(const Callback&) = 0;
    /*
      reset:
      function for reset the memory of the game
    */
    virtual void reset() = 0;

  public:
    /*
      tick:
      return vector of object each tick of times for update game
    */
    virtual ObjectList tick() = 0;
    /*
      eventHandling:
      handle event from graphic parts, this could be a key or resize but resize is not handled by games
    */
    virtual ObjectList handleEvent(const Event&) = 0;
    /*
      dump:
      return vector of all objects
    */
    virtual ObjectList dump() const = 0;

    /*
      dumpMemory:
      return the shared memory is library is a menu
      this should be accorded to the following format
      std::map<std::string, std::string> {
        libSelected: ...,
        gameSelected: ...,
        ...,
      }
    */
    virtual std::map<std::string, std::string> dumpMemory() const = 0;

    protected:
      Timer _timer;
  };
}

#endif
