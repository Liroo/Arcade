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
      DirectoryReader::DirectoryContent _availableLib;
      DirectoryReader::DirectoryContent _availableGame;

    public:
      virtual GameEvent start(const std::string&, const std::string&);
      virtual void reset();
      virtual GameEvent dump() const;
      virtual GameEvent tick();
      virtual GameEvent handleEvent(const Event&);
      virtual std::map<std::string, std::string> dumpMemory() const;

    private:
      Button _play;
      Button _pseudo;
      std::string _inputPseudo;
      Carroussel _game;
      Carroussel _graphic;
      Button _exit;
      AnimatedObject _vader;
      Object _background;

      Object _backgroundPopUp;
      Object _arrowPopUp;
      Button _personalScore;
      Button _scores;
      Button _availableLibPopUp;

      void _updatePopUpText();

      int _cursorPosition;
      ObjectList _objects;

      void _render();
      void _renderPopUp();

    public:
      bool _updateDirLib();

    private:
      std::map<KeyType, std::function<GameEvent()>> _key;

      GameEvent _keyDown();
      GameEvent _keyUp();
      GameEvent _keyLeft();
      GameEvent _keyRight();
      GameEvent _keyEnter();

      GameEvent _handleInput(const Event&);
  };
};

extern "C" {
  Arcade::AGames* load_lib() {
    return new Arcade::Menu;
  }
}

#endif
