#include "Snake.h"
#include <cmath>
#include <ctgmath>
#include <iostream>
#include <iomanip>

using namespace Arcade;

Snake::Snake() {}
Snake::~Snake() {
  _timer.stop();
}

GameEvent Snake::start() {
  _gameOver = false;
  _isPause = false;
  _score = 0;
  _foundCount = 0;
  _lastMovement = Arcade::KeyType::KEY_UP;
  _currentMovement  = Arcade::KeyType::KEY_UP;
  _speed = 500;
  _lastTime = 0;
  _timer.start();
  _snake = {
    { 14, 14 },
    { 14, 15 },
    { 14, 16 },
    { 14, 17 }
  };
  return {
    Arcade::EventType::DISPLAY,
    _objects
  };
}

void Snake::reset() {
  _snake = {
    { 14, 14 },
    { 14, 15 },
    { 14, 16 },
    { 14, 17 }
  };
  _gameOver = false;
  _isPause = false;
  _lastTime = 0;
  _score = 0;
  _foundCount = 0;
  _lastMovement = Arcade::KeyType::KEY_UP;
  _currentMovement  = Arcade::KeyType::KEY_UP;
  _timer.reset();
}

GameEvent Snake::dump() const {
  return {
    Arcade::EventType::DISPLAY,
    _objects
  };
}

GameEvent Snake::tick() {
  _move();
  for (auto it = _snake.begin(); it < _snake.end(); it++) {
    std::cout << std::fixed << std::setprecision(3) << (*it).first << ":" << (*it).second << std::endl;
  }
  std::cout << std::endl;
  return {
    Arcade::EventType::DISPLAY,
    _objects
  };
}

GameEvent Snake::handleEvent(const Event& event) {
  switch (event.key) {
    case Arcade::KeyType::KEY_DOWN:
      _lastMovement = Arcade::KeyType::KEY_DOWN;
      break;
    case Arcade::KeyType::KEY_UP:
      _lastMovement = Arcade::KeyType::KEY_UP;
      break;
    case Arcade::KeyType::KEY_LEFT:
      _lastMovement = Arcade::KeyType::KEY_LEFT;
      break;
    case Arcade::KeyType::KEY_RIGHT:
      _lastMovement = Arcade::KeyType::KEY_RIGHT;
      break;
    default:
      break;
  };
  return {
    Arcade::EventType::DISPLAY,
    _objects
  };
}

std::map<std::string, std::string> Snake::dumpMemory() const {
  return {};
}

void Snake::_spawnFruit() {}

void Snake::_move() {
  int fullTimeSpent = _timer.getTick();
  std::cout << fullTimeSpent << std::endl;
  int deltaTime = fullTimeSpent - _lastTime;
  _lastTime = fullTimeSpent;

  float moveRange = (float)deltaTime / (float)_speed;
  std::cout << std::fixed << std::setprecision(3) << deltaTime << ":" << moveRange << std::endl;
}
