#include "Snake.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>
// debug header
#include <iostream>
#include <iomanip>

# include "exceptions/ArcadeException.hpp"

using namespace Arcade;

Snake::Snake():
  _titleButton("title"),
  _timeButton("time"),
  _scoreButton("score"),
  _car("car", std::string(DIR_RESSOURCES) + "animated_car/") {

  _background.id = "background";
  _background.position = { 0, 0 };
  _background.size = { 1280, 720 };
  _background.backgroundColor = 0xC89123;
  _background.elevation = 1;

  _board.id = "board";
  _board.position = { 60, 60 };
  _board.size = { 600, 600 };
  _board.backgroundColor = 0x4F4D39;
  _board.elevation = 7;

  _boardWall.id = "boardWall";
  _boardWall.position = { 40, 40 };
  _boardWall.size = { 640, 640 };
  _boardWall.backgroundColor = 0x7C7A6B;
  _boardWall.elevation = 6;

  _titleButton.setSize({ 430, 80 });
  _titleButton.setPosition({ 755, 60 });
  _titleButton.setBackgroundColor(0xFF6951);
  _titleButton.setShadowColor(0xC75959);
  _titleButton.setFontSize(42);
  _titleButton.setText("Nibbler");

  _timeButton.setSize({ 430, 80 });
  _timeButton.setPosition({ 755, 190 });
  _timeButton.setBackgroundColor(0x4D519F);
  _timeButton.setShadowColor(0x6759C7);
  _timeButton.setFontSize(42);
  _timeButton.setText("00:00");

  _scoreButton.setSize({ 430, 80 });
  _scoreButton.setPosition({ 755, 575 });
  _scoreButton.setBackgroundColor(0x79A05F);
  _scoreButton.setShadowColor(0x628A47);
  _scoreButton.setFontSize(42);
  _scoreButton.setText("00");

  _car.setSize({ 505, 245 });
  _car.setPosition({ 753, 305 });

  if (!_car.init()) {
    throw Arcade::Exception::ArcadeException(ERR_SNAKE_INIT); // ERROR
  }

  _gameOver = false;
  _isPause = false;
  _score = 0;
  _fruitCount = 0;
  _lastKey = _currentKey = Arcade::KeyType::KEY_UP;
  _movement = _nextMovement = { 0, -1 };
  _speed = 150;
  _toIncrease = 0;
  _lastTime = 0;
  _snake = {
    { 14.0, 14.0 },
    { 14.0, 15.0 },
    { 14.0, 16.0 },
    { 14.0, 17.0 }
  };
  _fruit.clear();
  for (int i = 0; i < 30; i++) {
    for (int j = 0; j < 30; j++) {
      _emptyMap.push_back({ i, j });
    }
  }
}

Snake::~Snake() {
  _timer.stop();
}

GameEvent Snake::start(const std::string& libName, const std::string& pseudo) {
  _pseudo = pseudo;
  _libName = libName;
  _timer.start();
  std::srand(std::time(0));

  appendObjectToList(_objects, _background);
  appendObjectToList(_objects, _boardWall);
  appendObjectToList(_objects, _board);
  appendListToList(_objects, _titleButton.render());
  return {
    Arcade::EventType::DISPLAY,
    _objects
  };
}

void Snake::reset() {
  _snake = {
    { 14.0, 14.0 },
    { 14.0, 15.0 },
    { 14.0, 16.0 },
    { 14.0, 17.0 }
  };
  _speed = 150;
  _gameOver = false;
  _isPause = false;
  _lastTime = 0;
  _score = 0;
  _fruitCount = 0;
  _fruit.clear();
  _toIncrease = 0;
  _lastKey = _currentKey = Arcade::KeyType::KEY_UP;
  _movement = _nextMovement = { 0, -1 };
  _timer.reset();
  std::srand(std::time(0));
  _objects.clear();

  appendObjectToList(_objects, _background);
  appendObjectToList(_objects, _boardWall);
  appendObjectToList(_objects, _board);
  appendListToList(_objects, _titleButton.render());
}

GameEvent Snake::dump() const {
  return {
    Arcade::EventType::DISPLAY,
    _objects
  };
}

GameEvent Snake::tick() {
  if (!_gameOver && !_isPause) {
    _spawnFruit();
    _move();
  }
  _timeButton.setText( std::to_string(_timer.getTick() / 1000 / 60)
    + ":"
    + std::to_string(_timer.getTick() / 1000 % 60) );
  if (_gameOver) {
    _timeButton.setText("GAME OVER");
  } else if (_isPause) {
    _timeButton.setText("PAUSE");
  }
  appendListToList(_objects, _timeButton.render());
  _scoreButton.setText(std::to_string(_score));
  appendListToList(_objects, _scoreButton.render());
  appendObjectToList(_objects, _car.renderNext((double)(_timer.getTick() + 100) * 1.5));
  if (!_gameOver) {
    _serializeSnake();
    _serializeFruit();
  }
  return {
    Arcade::EventType::DISPLAY,
    _objects
  };
}

void Snake::_serializeSnake() {
  // calculate and set time
  int fullTimeSpent = _timer.getTick();
  int deltaTime = fullTimeSpent - _lastTime;

  Arcade::Object baseObject;

  baseObject.size = { 20, 20 };
  baseObject.elevation = 30;
  baseObject.backgroundColor = -1;
  baseObject.id = std::string("snake:head:0");
  std::pair<int, int> direction = _movement;
  std::pair<int, int> lastCase = _snake[0];
  baseObject.position = {
    60 + 20 * _snake[0].first + (direction.first * (deltaTime * 20 / _speed)),
    60 + 20 * _snake[0].second + (direction.second * (deltaTime * 20 / _speed))
  };
  baseObject.imageName = std::string(DIR_RESSOURCES) + "head.png";
  appendObjectToList(_objects, baseObject);

  baseObject.imageName = std::string(DIR_RESSOURCES) + "body.png";
  int i = 0;
  for (auto it = _snake.begin() + 1; it < _snake.end(); it++) {
    baseObject.id = std::string("snake:body:") + std::to_string(i);
    direction = {
      lastCase.first - (*it).first,
      lastCase.second - (*it).second
    };
    baseObject.position = {
      60 + 20 * (*it).first + (direction.first * (deltaTime * 20 / _speed)),
      60 + 20 * (*it).second + (direction.second * (deltaTime * 20 / _speed))
    };
    appendObjectToList(_objects, baseObject);
    lastCase = *it;
    i++;
  }
}

void Snake::_serializeFruit() {
  Arcade::Object baseObject;

  baseObject.size = { 20, 20 };
  baseObject.elevation = 29;
  baseObject.backgroundColor = -1;
  baseObject.imageName = std::string(DIR_RESSOURCES) + "apple.png";
  int i = 0;
  for (auto it = _fruit.begin(); it < _fruit.end(); it++) {
    baseObject.id = std::string("snake:fruit:") + std::to_string(i);
    baseObject.position = {
      60 + 20 * (*it).first,
      60 + 20 * (*it).second
    };
    appendObjectToList(_objects, baseObject);
    i++;
  }
}

GameEvent Snake::handleEvent(const Event& event) {
  switch (event.key) {
    case Arcade::KeyType::KEY_DOWN:
      if (_currentKey != Arcade::KeyType::KEY_UP) {
        _lastKey = Arcade::KeyType::KEY_DOWN;
        _nextMovement = { 0, 1 };
      }
      break;
    case Arcade::KeyType::KEY_UP:
      if (_currentKey != Arcade::KeyType::KEY_DOWN) {
        _lastKey = Arcade::KeyType::KEY_UP;
        _nextMovement = { 0, -1 };
      }
      break;
    case Arcade::KeyType::KEY_RIGHT:
      if (_currentKey != Arcade::KeyType::KEY_LEFT) {
        _lastKey = Arcade::KeyType::KEY_RIGHT;
        _nextMovement = { 1, 0 };
      }
      break;
    case Arcade::KeyType::KEY_LEFT:
      if (_currentKey != Arcade::KeyType::KEY_RIGHT) {
        _lastKey = Arcade::KeyType::KEY_LEFT;
        _nextMovement = { -1, 0 };
      }
      break;
    case Arcade::KeyType::KEY_UNKNOWN:
      if (event.data == 16) {
        if (_isPause) {
          _timer.resume();
          _isPause = false;
        } else {
          _timer.pause();
          _isPause = true;
        }
      }
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

void Snake::_spawnFruit() {
  if (_fruitCount < ((int)_snake.size() / 4)) {
    std::vector<std::pair<int, int>> tmpMap = _emptyMap;
    for (auto it = _snake.begin(); it < _snake.end(); it++) {
      tmpMap.erase(std::find(tmpMap.begin(), tmpMap.end(), *it));
    }
    _fruit.push_back(tmpMap[std::rand() % tmpMap.size()]);
    _fruitCount++;
  }
}

void Snake::_move() {
  // calculate and set time
  int fullTimeSpent = _timer.getTick();
  int deltaTime = fullTimeSpent - _lastTime;

  // move to the next case
  if (deltaTime < _speed) {
    return;
  }
  std::pair<int, int> previous = _snake[0];
  std::pair<int, int> previous2;
  _snake[0] = {
    _snake[0].first + _movement.first,
    _snake[0].second + _movement.second
  };
  for (auto it = _snake.begin() + 1; it < _snake.end(); it++) {
    previous2 = *it;
    *it = previous;
    previous = previous2;
  }
  if (_toIncrease > 0) {
    _snake.push_back(previous2);
    _toIncrease -= 1;
  }
  auto doesTouch = std::find(_snake.begin() + 1, _snake.end(), _snake[0]);
  if (doesTouch != _snake.end()) {
    _gameOver = true;
    return;
  }
  _lastTime = fullTimeSpent;
  _movement = _nextMovement;

  auto it = std::find(_fruit.begin(), _fruit.end(), _snake[0]);
  if (it != _fruit.end()) {
    if (_snake.size() > 50) {
      _speed = 50;
    } else if (_snake.size() > 40) {
      _speed = 85;
    } else {
      _speed -= 1;
    }
    _fruit.erase(it);
    _score += (160 - _speed);
    _toIncrease += (std::rand() % 3) + 1;
    _fruitCount--;
  }
  if (_snake[0].first < 0 || _snake[0].first >= MAP_WIDTH
    || _snake[0].second < 0 || _snake[0].second >= MAP_HEIGHT) {
      _gameOver = true;
    }
  _currentKey = _lastKey;
}
