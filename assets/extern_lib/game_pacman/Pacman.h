#ifndef PACMAN_H
# define PACMAN_H

# include <string>
# include <sstream>
# include <fstream>
# include <iostream>
# include <iterator>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <time.h>

# include "components/Button.h"
# include "components/AnimatedObject.h"
# include "DirectoryReader.h"
# include "Square.h"
# include "Ghost.h"
# include "Player.h"
# include "AGames.h"

namespace Arcade {
  class Pacman : public AGames {

    private:
      int _score;
      int _speed;
      int _lastTime;
      bool _gameOver;
      bool _isPause;
      Player _pacpac;
      Button _titleButton;
      Button _timeButton;
      Button _scoreButton;
      AnimatedObject _pac;
      Object _boardWall;
      ObjectList _objects;
      std::string _libName;
      std::string _pseudo;
      std::vector<std::string> _map;
      std::vector<Ghost> _enemies;
      std::vector<std::string> _cellToPass;
      Object _background;
      Callback _callback;

    public:
      Pacman();
      virtual ~Pacman();

    public:
        virtual GameEvent start(const std::string&, const std::string&);
        void play();
        virtual void reset();
        virtual GameEvent dump() const;
        virtual GameEvent tick();
        virtual GameEvent handleEvent(const Event&);
        virtual std::map<std::string, std::string> dumpMemory() const;

    private:
      std::map<KeyType, std::function<GameEvent()>> _key;

      GameEvent _keyDown();
      GameEvent _keyUp();
      GameEvent _keyLeft();
      GameEvent _keyRight();

    public:
      void createMap();
      Ghost moveGhost(Ghost&);
      bool checkCell();
      char getInMap(const std::pair<int, int>&) const;
      void movePlayer(const int&);
      void setInMap(const std::pair<int, int>&, const char&);
      Arcade::GameEvent cleanTables();
      void drawMap();
  };
};

extern "C" {
  Arcade::AGames* load_lib() {
    return new Arcade::Pacman;
  }
}

#endif
