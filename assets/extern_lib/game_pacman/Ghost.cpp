#include "Ghost.h"

Arcade::Ghost::Ghost(const std::string &id, const std::string &image){
  _position = { 10, 8 };
  _size = {30, 30};
  _inMap = 'G';
  _id = id;
  _imageName = image;
  _elevation = 50;
  _direction = Arcade::DirectionType::DIRECTION_DEFAULT;
}

Arcade::Ghost &Arcade::Ghost::operator=(const Arcade::Ghost &ghost) {
  _position = ghost._position;
  _size = ghost._size;
  _direction = ghost._direction;
  _id = ghost._id;
  _inMap = ghost._inMap;
  _elevation = ghost._elevation;
  _imageName = ghost._imageName;
  _object = ghost._object;
  return *this;
}

std::pair<int, int> Arcade::Ghost::getPosition() const {
  return _position;
}

void Arcade::Ghost::setPosition(const std::pair<int, int> &position) {
  _position = position;
}

std::pair<int, int> Arcade::Ghost::getSize() const {
  return _size;
}

void Arcade::Ghost::setSize(const std::pair<int, int> &size) {
  _size = size;
}

std::string Arcade::Ghost::getImageName() const {
  return _imageName;
}

void Arcade::Ghost::setImageName(const std::string &imageName) {
  _imageName = imageName;
}

char Arcade::Ghost::getInMap() const {
  return _inMap;
}

void Arcade::Ghost::setInMap(const char &c) {
  _inMap = c;
}

int Arcade::Ghost::getDirection() const {
  return _direction;
}

void Arcade::Ghost::setDirection(const Arcade::DirectionType &direction) {
  _direction = direction;
}

std::string Arcade::Ghost::getId() const {
  return _id;
}

std::pair<int, int> Arcade::Ghost::getRawPosition() const {
  return _rawPosition;
}

void Arcade::Ghost::setRawPosition(const std::pair<int, int>&rawPosition) {
  _rawPosition = rawPosition;
}

Arcade::Object Arcade::Ghost::render() {
  _object.rawPosition = _position;
  _object.position = {45 + _position.first * 30, 45 + _position.second * 30};
  _object.size = _size;
  _object.elevation = _elevation;
  _object.id = _id;
  _object.imageName = _imageName;
  _object.rawImage.push_back("X");
  return _object;
}
