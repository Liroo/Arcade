#include "Pacman.h"

#include "exceptions/ArcadeException.hpp"
#include "components/Score.h"

using namespace Arcade;

Pacman::Pacman():
  _pacpac("pacman:pacman", std::string(DIR_RESSOURCES) + "pacman/pacman.png"),
  _titleButton("title"),
  _timeButton("time"),
  _scoreButton("score"),
  _pac("walkPac", std::string(DIR_RESSOURCES) + "animated_pacman/") {
  // pointer handler to handleEvent
  _key[KeyType::KEY_DOWN] = [this]() -> GameEvent {
    return _keyDown();
  };
  _key[KeyType::KEY_UP] = [this]() -> GameEvent {
    return _keyUp();
  };
  _key[KeyType::KEY_LEFT] = [this]() -> GameEvent {
    return _keyLeft();
  };
  _key[KeyType::KEY_RIGHT] = [this]() -> GameEvent {
    return _keyRight();
  };

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
  _titleButton.setText("Pacman");

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

  _pac.setSize({ 290, 220 });
  _pac.setPosition({ 828, 305 });

  if (!_pac.init()) {
    throw Exception::ArcadeException("Error init Pacman"); // ERROR
  }

  _gameOver = false;
  _isPause = false;
  _score = 0;
  createMap();
}

Pacman::~Pacman() {
  _timer.stop();
}

GameEvent Pacman::start(const std::string &libName, const std::string &pseudo) {
  Ghost ghost1("pacman:yellow", std::string(DIR_RESSOURCES) + "pacman/yellow.png");
  Ghost ghost2("pacman:pink", std::string(DIR_RESSOURCES) + "pacman/pink.png");
  Ghost ghost3("pacman:blue", std::string(DIR_RESSOURCES) + "pacman/blue.png");
  Ghost ghost4("pacman:red", std::string(DIR_RESSOURCES) + "pacman/red.png");

  appendObjectToList(_objects, _boardWall);
  appendListToList(_objects, _titleButton.render());
  _libName = libName;
  _pseudo = pseudo;
  _speed = 200;
  _lastTime = 0;
  _timer.start();
  _enemies.push_back(ghost1);
  _enemies.push_back(ghost2);
  _enemies.push_back(ghost3);
  _enemies.push_back(ghost4);

  _background.position = { 0, 0 };
  _background.size = { 1280, 720 };
  _background.backgroundColor = 0xffffff;
  _background.elevation = 1;
  _background.id = "background";
  appendObjectToList(_objects, _background);

  drawMap();

  _enemies.at(0).setPosition({10, 8});
  _enemies.at(1).setPosition({10, 9});
  _enemies.at(2).setPosition({9, 9});
  _enemies.at(3).setPosition({11, 9});
  setInMap({ 10, 8 }, 'G');
  setInMap({ 10, 9 }, 'G');
  setInMap({ 9, 9 }, 'G');
  setInMap({ 11, 9 }, 'G');
  appendObjectToList(_objects, _pacpac.render());
  for (std::vector<Ghost>::iterator it = _enemies.begin(); it != _enemies.end(); it++) {
    appendObjectToList(_objects, (*it).render());
  }
  return {
    EventType::DISPLAY,
    _objects
  };
}

void Pacman::createMap() {
  _map.push_back("111111111111111111111");
  _map.push_back("100000000010000000001");
  _map.push_back("101110111010111011101");
  _map.push_back("100000000000000000001");
  _map.push_back("101110101111101011101");
  _map.push_back("100000100010001000001");
  _map.push_back("111110111010111011111");
  _map.push_back("111110100000001011111");
  _map.push_back("111110101121101011111");
  _map.push_back("111110001222100011111");
  _map.push_back("111110101111101011111");
  _map.push_back("111110100000001011111");
  _map.push_back("111110101111101011111");
  _map.push_back("100000000010000000001");
  _map.push_back("101110111010111011101");
  _map.push_back("100010000000000010001");
  _map.push_back("111010101111101010111");
  _map.push_back("100000100010001000001");
  _map.push_back("101111111010111111101");
  _map.push_back("100000000000000000001");
  _map.push_back("111111111111111111111");
  for (std::vector<std::string>::iterator it = _map.begin(); it != _map.end(); it++) {
    _cellToPass.push_back((*it));
  }
}

void Pacman::drawMap() {
  int y;
  int cmpt;

  y = 0;
  cmpt = 0;
  for (std::vector<std::string>::iterator it = _map.begin(); it != _map.end(); it++)
    {
      for (std::string::iterator it2 = _map[y].begin(); it2 != _map[y].end(); it2++) {
        if (*it2 == '1')
        {
          Square newSquare("pacman:wall:"+std::to_string(cmpt));
          newSquare.setPosition({(45 + (std::distance(_map[y].begin(), it2) * 30)), (45 + (y * 30))});
          newSquare.setBackgroundColor(0x082ab2);
          appendObjectToList(_objects, newSquare.render());
        }
        else if (*it2 == '2') {
          Square fence("pacman:fence:"+std::to_string(cmpt));
          fence.setPosition({(45 + (std::distance(_map[y].begin(), it2) * 30)), (45 + (y * 30))});
          fence.setBackgroundColor(0xF1A59B);
          fence.setSize({30, 10});
          appendObjectToList(_objects, fence.render());
        }
        else {
          Square dotSquare("pacman:dot:"+std::to_string(cmpt));
          dotSquare.setBackgroundColor(0x000000);
          dotSquare.setPosition({(55 + (std::distance(_map[y].begin(), it2) * 30)), (55 + (y * 30))});
          dotSquare.setSize({5, 5});
          dotSquare.setImage(std::string(DIR_RESSOURCES) + "pacman/dot.png");
          appendObjectToList(_objects, dotSquare.render());
        }
        cmpt++;
      }
      y++;
    }
}

GameEvent Pacman::cleanTables() {
  _map.clear();
  _cellToPass.clear();
  createMap();
  drawMap();
  _pacpac.setPosition({10, 11});
  _speed -= 100;
  _enemies.at(0).setPosition({10, 8});
  _enemies.at(1).setPosition({10, 9});
  _enemies.at(2).setPosition({9, 9});
  _enemies.at(3).setPosition({11, 9});
  setInMap({ 10, 8 }, 'G');
  setInMap({ 10, 9 }, 'G');
  setInMap({ 9, 9 }, 'G');
  setInMap({ 11, 9 }, 'G');
  appendObjectToList(_objects, _pacpac.render());
  for (std::vector<Ghost>::iterator it = _enemies.begin(); it != _enemies.end(); it++) {
    (*it).setDirection(DirectionType::DIRECTION_DEFAULT);
    appendObjectToList(_objects, (*it).render());
  }
  return {
    EventType::DISPLAY,
    _objects
  };
}

void Pacman::reset() {
  _score = 0;
  _gameOver = false;
  _isPause = false;
  _lastTime = 0;
  _speed = 200;
  _map.clear();
  _cellToPass.clear();
  createMap();
  drawMap();
  _pacpac.setPosition({10, 11});
  _enemies.at(0).setPosition({10, 8});
  _enemies.at(1).setPosition({10, 9});
  _enemies.at(2).setPosition({9, 9});
  _enemies.at(3).setPosition({11, 9});
  setInMap({ 10, 8 }, 'G');
  setInMap({ 10, 9 }, 'G');
  setInMap({ 9, 9 }, 'G');
  setInMap({ 11, 9 }, 'G');
  appendObjectToList(_objects, _pacpac.render());
  for (std::vector<Ghost>::iterator it = _enemies.begin(); it != _enemies.end(); it++) {
    (*it).setDirection(DirectionType::DIRECTION_DEFAULT);
    appendObjectToList(_objects, (*it).render());
  }
  _timer.reset();
}

void Pacman::play() {}

std::map<std::string, std::string> Pacman::dumpMemory() const {
  return {};
}

GameEvent Pacman::dump() const {
  return {
    EventType::DISPLAY,
    _objects
  };
}

Ghost Pacman::moveGhost(Ghost &ghost) {
  bool  isRand;
  int   r;
  int   limit;

  limit = 0;
  isRand = false;
  srand(time(NULL));
  while (isRand == false)
  {
    r = rand() % 5;
    limit++;
    switch(r) {
      case 0:
      if (_map.at(ghost.getPosition().second - 1).at(ghost.getPosition().first) != '1' &&
          _map.at(ghost.getPosition().second - 1).at(ghost.getPosition().first) != 'G') {
          _map.at(ghost.getPosition().second).at(ghost.getPosition().first) = '0';
          ghost.setPosition({(ghost.getPosition().first), ghost.getPosition().second - 1});
          if (_map.at(ghost.getPosition().second).at(ghost.getPosition().first) == 'P') {
              setScoreForGame(_libName, _pseudo, _score);
              _gameOver = true;
            }
          _map.at(ghost.getPosition().second).at(ghost.getPosition().first) = ghost.getInMap();
          ghost.setDirection(DirectionType::DIRECTION_UP);
          appendObjectToList(_objects, ghost.render());
          isRand = true;
        }
        break;
      case 1:
      if (_map.at(ghost.getPosition().second + 1).at(ghost.getPosition().first) != '1' &&
          _map.at(ghost.getPosition().second + 1).at(ghost.getPosition().first) != 'G') {
          _map.at(ghost.getPosition().second).at(ghost.getPosition().first) = '0';
          ghost.setPosition({(ghost.getPosition().first), ghost.getPosition().second + 1});
          if (_map.at(ghost.getPosition().second).at(ghost.getPosition().first) == 'P') {
              _gameOver = true;
            }
          _map.at(ghost.getPosition().second).at(ghost.getPosition().first) = ghost.getInMap();
          ghost.setDirection(DirectionType::DIRECTION_DOWN);
          appendObjectToList(_objects, ghost.render());
          isRand = true;
        }
        break;
      case 2:
      if (_map.at(ghost.getPosition().second).at(ghost.getPosition().first - 1) != '1' &&
          _map.at(ghost.getPosition().second).at(ghost.getPosition().first - 1) != 'G') {
          _map.at(ghost.getPosition().second).at(ghost.getPosition().first) = '0';
          ghost.setPosition({(ghost.getPosition().first - 1), ghost.getPosition().second});
          if (_map.at(ghost.getPosition().second).at(ghost.getPosition().first) == 'P')
          {
            setScoreForGame(_libName, _pseudo, _score);
            _gameOver = true;
          }
          _map.at(ghost.getPosition().second).at(ghost.getPosition().first) = ghost.getInMap();
          ghost.setDirection(DirectionType::DIRECTION_LEFT);
          appendObjectToList(_objects, ghost.render());
          isRand = true;
        }
        break;
      case 3:
      if (_map.at(ghost.getPosition().second).at(ghost.getPosition().first + 1) != '1' &&
          _map.at(ghost.getPosition().second).at(ghost.getPosition().first + 1) != 'G') {
          _map.at(ghost.getPosition().second).at(ghost.getPosition().first) = '0';
          ghost.setPosition({(ghost.getPosition().first + 1), ghost.getPosition().second});
          if (_map.at(ghost.getPosition().second).at(ghost.getPosition().first) == 'P')
          {
            setScoreForGame(_libName, _pseudo, _score);
            _gameOver = true;
          }
          _map.at(ghost.getPosition().second).at(ghost.getPosition().first) = ghost.getInMap();
          ghost.setDirection(DirectionType::DIRECTION_RIGHT);
          appendObjectToList(_objects, ghost.render());
          isRand = true;
        }
        break;
      };
      if (limit >= 4)
        isRand = true;
    }
  return ghost;
}

bool  Pacman::checkCell() {
  for (std::vector<std::string>::iterator it = _cellToPass.begin(); it != _cellToPass.end(); it++) {
    if ((*it).find('0') != std::string::npos)
    return false;
  }
  return true;
}

void Pacman::setInMap(const std::pair<int, int> &pos, const char &c) {
  _map.at(pos.second).at(pos.first) = c;
}

GameEvent Pacman::tick() {

  int fullTimeSpent = _timer.getTick();
  int deltaTime = fullTimeSpent - _lastTime;

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
  Square cell(std::to_string(8 * 21 + 10));
  cell.setPosition({(45 + 10 * 30), (45 + 8 * 30)});
  cell.setBackgroundColor(0xF1A59B);
  cell.setSize({30, 10});
  setInMap({ 10, 8 }, '2');
  appendObjectToList(_objects, cell.render());
  Square cell1(std::to_string(9 * 21 + 10));
  cell.setPosition({45 + (10 * 30), 45 + (8 * 30)});
  cell.setBackgroundColor(0x000000);
  cell.setImage("");
  appendObjectToList(_objects, cell1.render());
  Square cell2(std::to_string(9 * 21 + 9));
  cell.setPosition({45 + (10 * 30), 45 + (8 * 30)});
  cell.setBackgroundColor(0x000000);
  cell.setImage("");
  appendObjectToList(_objects, cell2.render());
  Square cell3(std::to_string(9 * 21 + 11));
  cell.setPosition({45 + (10 * 30), 45 + (8 * 30)});
  cell.setBackgroundColor(0x000000);
  cell.setImage("");
  appendObjectToList(_objects, cell3.render());
  appendObjectToList(_objects, _background);
  if (!_gameOver && !_isPause) {
    if (deltaTime >= _speed)
    {
      if (checkCell() == true) {
        cleanTables();
      }
      else {
          movePlayer(deltaTime);
          _lastTime = fullTimeSpent;
          if (fullTimeSpent > 5000)
          _enemies.at(0) = moveGhost(_enemies.at(0));
          if (fullTimeSpent > 10000)
          _enemies.at(1) = moveGhost(_enemies.at(1));
          if (fullTimeSpent > 15000)
          _enemies.at(2) = moveGhost(_enemies.at(2));
          if (fullTimeSpent > 20000)
          _enemies.at(3) = moveGhost(_enemies.at(3));
        }
      }
    }
  appendObjectToList(_objects, _pac.renderNext((double)(_timer.getTick() + 100) * 1.1));
  return {
    EventType::DISPLAY,
    _objects
  };
}

GameEvent Pacman::handleEvent(const Event& event) {
  try {
      if (event.key == KeyType::KEY_UNKNOWN)
        if (event.data == 16) {
          if (_isPause) {
            _timer.resume();
            _isPause = false;
          } else {
            _timer.pause();
            _isPause = true;
          }
        }
    return _key.at(event.key)();
  } catch (std::out_of_range) {
    return {};
  }
}

void Pacman::movePlayer(const int &deltaTime) {
  (void)deltaTime;
  if (_map.at(_pacpac.getPosition().second + 1).at(_pacpac.getPosition().first) &&
      _map.at(_pacpac.getPosition().second + 1).at(_pacpac.getPosition().first) == '0' &&
      _pacpac.getDirection() == DirectionType::DIRECTION_DOWN)
  {
    _map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) = '0';
    _pacpac.setPosition({_pacpac.getPosition().first, _pacpac.getPosition().second + 1});
    if (_map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) == 'G')
    {
      setScoreForGame(_libName, _pseudo, _score);
      _gameOver = true;
    }
    if (_cellToPass.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) == '0')
      _score += 10 + ((_speed - 200) / 2) * -1;
    _cellToPass.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) = _pacpac.getInMap();
    _map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) = _pacpac.getInMap();
    Square cell("pacman:dot:"+std::to_string(_pacpac.getPosition().second * 21 + _pacpac.getPosition().first));
    cell.setPosition({45 + (_pacpac.getPosition().first * 30), 45 + (_pacpac.getPosition().second * 30)});
    cell.setBackgroundColor(0x7C7A6B);
    cell.setImage("");
    appendObjectToList(_objects, cell.render());
    appendObjectToList(_objects, _pacpac.render());
    _pacpac.setDirection(DirectionType::DIRECTION_DOWN);
  }
  else if (_map.at(_pacpac.getPosition().second - 1).at(_pacpac.getPosition().first) &&
          _map.at(_pacpac.getPosition().second - 1).at(_pacpac.getPosition().first) == '0' &&
          _pacpac.getDirection() == DirectionType::DIRECTION_UP) {
            _map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) = '0';
            _pacpac.setPosition({_pacpac.getPosition().first, _pacpac.getPosition().second - 1});
            if (_map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) == 'G')
            {
              setScoreForGame(_libName, _pseudo, _score);
              _gameOver = true;
            }
            if (_cellToPass.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) == '0')
            _score += 10 + ((_speed - 200) / 2) * -1;
            _cellToPass.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) = _pacpac.getInMap();
            _map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) = _pacpac.getInMap();
            Square cell("pacman:dot:"+std::to_string(_pacpac.getPosition().second * 21 + _pacpac.getPosition().first));
            cell.setPosition({45 + (_pacpac.getPosition().first * 30), 45 + (_pacpac.getPosition().second * 30)});
            cell.setBackgroundColor(0x7C7A6B);
            cell.setImage("");
            appendObjectToList(_objects, cell.render());
            appendObjectToList(_objects, _pacpac.render());
            _pacpac.setDirection(DirectionType::DIRECTION_UP);
          }
  else if (_map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first - 1) &&
          _map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first - 1) == '0' &&
          _pacpac.getDirection() == DirectionType::DIRECTION_LEFT) {
            _map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) = '0';
            _pacpac.setPosition({_pacpac.getPosition().first - 1, _pacpac.getPosition().second});
            if (_map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) == 'G')
            {
              setScoreForGame(_libName, _pseudo, _score);
              _gameOver = true;
            }
            if (_cellToPass.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) == '0')
            _score += 10 + ((_speed - 200) / 2) * -1;
            _cellToPass.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) = _pacpac.getInMap();
            _map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) = _pacpac.getInMap();
            Square cell("pacman:dot:"+std::to_string(_pacpac.getPosition().second * 21 + _pacpac.getPosition().first));
            cell.setPosition({45 + (_pacpac.getPosition().first * 30), 45 + (_pacpac.getPosition().second * 30)});
            cell.setBackgroundColor(0x7C7A6B);
            cell.setImage("");
            appendObjectToList(_objects, cell.render());
            appendObjectToList(_objects, _pacpac.render());
            _pacpac.setDirection(DirectionType::DIRECTION_LEFT);
          }
  else if (_map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first + 1) &&
          _map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first + 1) == '0' &&
          _pacpac.getDirection() == DirectionType::DIRECTION_RIGHT) {
            _map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) = '0';
            _pacpac.setPosition({_pacpac.getPosition().first + 1, _pacpac.getPosition().second});
            if (_map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) == 'G')
            {
              setScoreForGame(_libName, _pseudo, _score);
              _gameOver = true;
            }
            if (_cellToPass.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) == '0')
            _score += 10 + ((_speed - 200) / 2) * -1;
            _cellToPass.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) = _pacpac.getInMap();
            _map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) = _pacpac.getInMap();
            Square cell("pacman:dot:"+std::to_string(_pacpac.getPosition().second * 21 + _pacpac.getPosition().first));
            cell.setPosition({45 + (_pacpac.getPosition().first * 30), 45 + (_pacpac.getPosition().second * 30)});
            cell.setBackgroundColor(0x7C7A6B);
            cell.setImage("");
            appendObjectToList(_objects, cell.render());
            appendObjectToList(_objects, _pacpac.render());
            _pacpac.setDirection(DirectionType::DIRECTION_RIGHT);
          }
}

GameEvent Pacman::_keyDown() {
  const float Pi = 3.141592654f;

  if (_map.at(_pacpac.getPosition().second + 1).at(_pacpac.getPosition().first) &&
      _map.at(_pacpac.getPosition().second + 1).at(_pacpac.getPosition().first) == '0' &&
      _pacpac.getDirection() != DirectionType::DIRECTION_DOWN && !_isPause && !_gameOver)
  {
    _map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) = '0';
    _pacpac.setPosition({_pacpac.getPosition().first, _pacpac.getPosition().second + 1});
    _pacpac.setRotation((float)0.5 * Pi);
    if (_map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) == 'G')
    {
      setScoreForGame(_libName, _pseudo, _score);
      _gameOver = true;
    }
    if (_cellToPass.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) == '0')
      _score += 10 + ((_speed - 200) / 2) * -1;
    _cellToPass.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) = _pacpac.getInMap();
    _map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) = _pacpac.getInMap();
    Square cell("pacman:dot:"+std::to_string(_pacpac.getPosition().second * 21 + _pacpac.getPosition().first));
    cell.setPosition({45 + (_pacpac.getPosition().first * 30), 45 + (_pacpac.getPosition().second * 30)});
    cell.setBackgroundColor(0x7C7A6B);
    cell.setImage("");
    appendObjectToList(_objects, cell.render());
    appendObjectToList(_objects, _pacpac.render());
    _pacpac.setDirection(DirectionType::DIRECTION_DOWN);
  }
  return {
    EventType::DISPLAY,
    _objects
  };
}

GameEvent Pacman::_keyUp() {
  const float Pi = 3.141592654f;
  if (_map.at(_pacpac.getPosition().second - 1).at(_pacpac.getPosition().first) &&
    _map.at(_pacpac.getPosition().second - 1).at(_pacpac.getPosition().first) == '0' &&
    _pacpac.getDirection() != DirectionType::DIRECTION_UP && !_isPause && !_gameOver)
  {
    _map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) = '0';
    _pacpac.setPosition({_pacpac.getPosition().first, _pacpac.getPosition().second - 1});
    _pacpac.setRotation((float)-0.5 * Pi);
    if (_map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) == 'G')
    {
      setScoreForGame(_libName, _pseudo, _score);
      _gameOver = true;
    }
    if (_cellToPass.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) == '0')
      _score += 10 + ((_speed - 200) / 2) * -1;
    _cellToPass.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) = _pacpac.getInMap();
    _map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) = _pacpac.getInMap();
    Square cell("pacman:dot:"+std::to_string(_pacpac.getPosition().second * 21 + _pacpac.getPosition().first));
    cell.setPosition({45 + (_pacpac.getPosition().first * 30), 45 + (_pacpac.getPosition().second * 30)});
    cell.setBackgroundColor(0x7C7A6B);
    cell.setImage("");
    appendObjectToList(_objects, cell.render());
    appendObjectToList(_objects, _pacpac.render());
    _pacpac.setDirection(DirectionType::DIRECTION_UP);
  }
  return {
    EventType::DISPLAY,
    _objects
  };
}

GameEvent Pacman::_keyLeft() {
  const float Pi = 3.141592654f;
  if (_map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first - 1) &&
    _map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first - 1) == '0' &&
    _pacpac.getDirection() != DirectionType::DIRECTION_LEFT && !_isPause && !_gameOver)
  {
    _map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) = '0';
    _pacpac.setPosition({_pacpac.getPosition().first - 1, _pacpac.getPosition().second});
    _pacpac.setRotation(Pi);
    if (_map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) == 'G')
    {
      setScoreForGame(_libName, _pseudo, _score);
      _gameOver = true;
    }
    if (_cellToPass.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) == '0')
      _score += 10 + ((_speed - 200) / 2) * -1;
    _cellToPass.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) = _pacpac.getInMap();
    _map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) = _pacpac.getInMap();
    Square cell("pacman:dot:"+std::to_string(_pacpac.getPosition().second * 21 + _pacpac.getPosition().first));
    cell.setPosition({45 + (_pacpac.getPosition().first * 30), 45 + (_pacpac.getPosition().second * 30)});
    cell.setBackgroundColor(0x7C7A6B);
    cell.setImage("");
    appendObjectToList(_objects, cell.render());
    appendObjectToList(_objects, _pacpac.render());
    _pacpac.setDirection(DirectionType::DIRECTION_LEFT);
  }
  return {
    EventType::DISPLAY,
    _objects
  };
}

GameEvent Pacman::_keyRight() {
  if (_map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first + 1) &&
      _map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first + 1) == '0' &&
      _pacpac.getDirection() != DirectionType::DIRECTION_RIGHT && !_isPause && !_gameOver)
  {
    _map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) = '0';
    _pacpac.setPosition({_pacpac.getPosition().first + 1, _pacpac.getPosition().second});
    _pacpac.setRotation(0);
    if (_map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) == 'G')
    {
      setScoreForGame(_libName, _pseudo, _score);
      _gameOver = true;
    }
    if (_cellToPass.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) == '0')
      _score += 10 + ((_speed - 200) / 2) * -1;
    _cellToPass.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) = _pacpac.getInMap();
    _map.at(_pacpac.getPosition().second).at(_pacpac.getPosition().first) = _pacpac.getInMap();
    Square cell("pacman:dot:"+std::to_string(_pacpac.getPosition().second * 21 + _pacpac.getPosition().first));
    cell.setPosition({45 + (_pacpac.getPosition().first * 30), 45 + (_pacpac.getPosition().second * 30)});
    cell.setBackgroundColor(0x7C7A6B);
    cell.setImage("");
    appendObjectToList(_objects, cell.render());
    appendObjectToList(_objects, _pacpac.render());
    _pacpac.setDirection(DirectionType::DIRECTION_RIGHT);
  }
  return {
    EventType::DISPLAY,
    _objects
  };
}

char  Pacman::getInMap(const std::pair<int, int> &position) const {
  return _map.at(position.second).at(position.first);
}
