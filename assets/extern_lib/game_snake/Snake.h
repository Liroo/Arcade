#ifndef SNAKE_H
# define SNAKE_H

# include <vector>
# include <utility>
# include "AGames.h"

# define MAP_WIDTH 30
# define MAP_HEIGHT 30

# define SQUARE_BORDER_SIZE 18

namespace Arcade {
  class Snake: public AGames {
    public:
      Snake();
      virtual ~Snake();

    public:
      virtual GameEvent start();
      virtual void reset();
      virtual GameEvent dump() const;
      virtual GameEvent tick();
      virtual GameEvent handleEvent(const Event&);
      virtual std::map<std::string, std::string> dumpMemory() const;

    private:
      ObjectList _objects;

    // only game logic
    private:
      bool _gameOver;
      bool _isPause;
      int _score;

      Arcade::KeyType _currentMovement;
      Arcade::KeyType _lastMovement;
      std::vector<std::pair<float, float>> _snake;
      std::vector<std::pair<int, int>> _fruit;
      int _lastTime;
      int _speed;
      void _move();

      int _foundCount;
      void _spawnFruit();
  };
};

extern "C" {
  Arcade::AGames* load_lib() {
    return new Arcade::Snake;
  }
}

#endif
