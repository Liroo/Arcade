#include "square.h"

using namespace Arcade;

Square::Square(const int &id) {
  _id = id;
  _bg = 0x0a29a8;
  _elevation = 20;
  _imageName = "";
  _rotation = 1;
}

Square::Square(const std::pair<int, int> &pos, const std::pair<int, int> &size, const int &id) {
  _pos = pos;
  _size = size;
  _id = id;
  _bg = 0x0a29a8;
  _elevation = 20;
  _imageName = "";
  _rotation = 1;
}

std::pair<int, int> Square::getPostion() const {
  return _pos;
}

void Square::setPostion(const std::pair<int, int> &pos) {
  _pos = pos;
}

std::pair<int, int> Square::getSize() const {
  return _size;
}

void Square::setSize(const std::pair<int, int> &size) {
  _size = size;
}

std::string Square::getImage() const {
  return _imageName;
}

void Square::setImage(const std::string &img) {
  _imageName = img;
}

int Square::getBackgroundColor() const {
  return _bg;
}

void Square::setBackgroundColor(const int &bg) {
  _bg = bg;
}

int Square::getRotation() const {
  return _rotation;
}

void Square::setRotation(const int &bg) {
  _rotation = rotation;
}

Object Square::render() {
  _squareObj.position = _pos;
  _squareObj.size = _size;
  _squareObj.rotation = _rotation;
  _squareObj.backgroundColor = _bg;
  _squareObj.id = _id;
  _squareObj.elevation = _elevation;
  _squareObj.imageName = _imageName;
  return _squareObj;
}
