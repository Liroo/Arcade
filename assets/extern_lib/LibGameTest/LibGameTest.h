#ifndef LIBGAMETEST_H
# define LIBGAMETEST_H

# include <memory>
# include "IGames.h"

class LibGameTest: public Arcade::IGames {
  public:
    LibGameTest();
    virtual ~LibGameTest();

  public:
    virtual void start();
    virtual void reset();

    virtual Arcade::ObjectList tick();
    virtual Arcade::ObjectList handleEvent(const Arcade::Event&);
    virtual Arcade::ObjectList dump() const;
    virtual std::map<std::string, std::string> dumpMemory() const;

  private:
    Arcade::ObjectList _objectListTest;
    std::map<std::string, std::string> _memoryTest;
};

extern "C" {
  Arcade::IGames* load_lib() {
    return new LibGameTest;
  }
}

#endif
