#include "Player.h"

Arcade::Player::Player(const std::string &id, const std::string &image){
  _position = { 10, 11 };
  _inMap = 'P';
  _size = {30, 30};
  _id = id;
  _direction = DIRECTION_DEFAULT;
  _imageName = image;
  _elevation = 50;
  _rotation = 0;
  _rawPosition = { 10, 11 };
}

std::pair<int, int> Arcade::Player::getPosition() const {
  return _position;
}

void Arcade::Player::setPosition(const std::pair<int, int> &position) {
  _position = position;
}

std::pair<int, int> Arcade::Player::getSize() const {
  return _size;
}

void Arcade::Player::setSize(const std::pair<int, int> &size) {
  _size = size;
}

std::string Arcade::Player::getImageName() const {
  return _imageName;
}

void Arcade::Player::setImageName(const std::string &imageName) {
  _imageName = imageName;
}

char Arcade::Player::getInMap() const {
  return _inMap;
}

void Arcade::Player::setInMap(const char &c) {
  _inMap = c;
}

int Arcade::Player::getDirection() const {
  return _direction;
}

void Arcade::Player::setDirection(const Arcade::DirectionType &direction) {
  _direction = direction;
}

std::string Arcade::Player::getId() const {
  return _id;
}

void Arcade::Player::setId(const std::string &id) {
  _id = id;
}

float Arcade::Player::getRotation() const {
  return _rotation;
}

void Arcade::Player::setRotation(const int &rotation) {
  _rotation = rotation;
}

std::pair<int, int> Arcade::Player::getRawPosition() const {
  return _rawPosition;
}

void Arcade::Player::setRawPosition(const std::pair<int, int>&rawPosition) {
  _rawPosition = rawPosition;
}

Arcade::Object Arcade::Player::render() {
  _object.rawPosition = _position;
  _object.position = {45 + _position.first * 30, 45 + _position.second * 30};
  _object.size = _size;
  _object.elevation = _elevation;
  _object.id = _id;
  _object.imageName = _imageName;
  _object.imageRotation = _rotation;
  _object.rawImage.push_back("C");
  return _object;
}
