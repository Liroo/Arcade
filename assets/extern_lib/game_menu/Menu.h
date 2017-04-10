#ifndef MENU_H
# define MENU_H

#include <functional>
#include "AGames.h"
#include "DirectoryReader.h"
#include "components/Button.h"
#include "components/Carroussel.h"
#include "components/AnimatedObject.h"

# define ERR_STARTEDLIB "This lib is not compatible or does not exist: "

namespace Arcade {
  class Menu: public AGames {
    public:
      Menu();
      virtual ~Menu();

    private:
      Button _play;
      Carroussel _game;
      Carroussel _graphic;
      Button _exit;
      AnimatedObject _vader;
      Object _background;

      int _cursorPosition;
      ObjectList _objects;

    private:
      DirectoryReader::DirectoryContent _availableLib;
      DirectoryReader::DirectoryContent _availableGame;

    public:
      virtual ObjectList start(const Callback&);
      virtual void reset();
      virtual ObjectList dump() const;
      virtual ObjectList tick();
      virtual ObjectList handleEvent(const Event&);
      virtual std::map<std::string, std::string> dumpMemory() const;

    public:
      bool _updateDirLib();

    private:
      Callback _callback;
      std::map<KeyType, std::function<ObjectList()>> _key;

      ObjectList _keyDown();
      ObjectList _keyUp();
      ObjectList _keyLeft();
      ObjectList _keyRight();
      ObjectList _keyEnter();
  };
};

extern "C" {
  Arcade::AGames* load_lib() {
    return new Arcade::Menu;
  }
}

#endif
