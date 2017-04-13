#ifndef SNAKE_H
# define SNAKE_H

# include <vector>
# include <utility>
# include "AGames.h"
# include "components/Button.h"
# include "components/AnimatedObject.h"

# define ERR_SNAKE_INIT "Snake failed to init"

# define MAP_WIDTH 30
# define MAP_HEIGHT 30

# define SQUARE_BORDER_SIZE 20

namespace Arcade {
  class Snake: public AGames {
    public:
      Snake();
      virtual ~Snake();

    public:
      virtual GameEvent start(const std::string&, const std::string&);
      virtual void reset();
      virtual GameEvent dump() const;
      virtual GameEvent tick();
      virtual GameEvent handleEvent(const Event&);
      virtual std::map<std::string, std::string> dumpMemory() const;

    private:
      std::string _pseudo;
      std::string _libName;

    private:
      Object _background;
      Object _boardWall;
      Object _board;
      Button _titleButton;
      Button _timeButton;
      Button _scoreButton;
      AnimatedObject _car;
      ObjectList _objects;
      void _serializeSnake();
      void _serializeFruit();

    // only game logic
    private:
      bool _gameOver;
      bool _isPause;
      int _score;

      Arcade::KeyType _lastKey;
      Arcade::KeyType _currentKey;
      std::pair<int, int> _movement;
      std::pair<int, int> _nextMovement;
      int _toIncrease;
      std::vector<std::pair<int, int>> _snake;
      std::vector<std::pair<int, int>> _fruit;
      int _lastTime;
      int _speed;
      void _move();

      int _fruitCount;
      void _spawnFruit();

      std::vector<std::pair<int, int>> _emptyMap;
  };
};

extern "C" {
  Arcade::AGames* load_lib() {
    return new Arcade::Snake;
  }
}

#endif
