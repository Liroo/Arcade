#include "Square.h"

using namespace Arcade;

Square::Square(const std::string &id) {
  _obj.id = id;
  _obj.backgroundColor = -1;
  _obj.elevation = 20;
  _obj.imageName = "";
  _obj.imageRotation = 0.0;
}

Square::Square(const std::pair<int, int> &pos, const std::pair<int, int> &size, const std::string &id) {
  _obj.position = pos;
  _obj.size = size;
  _obj.id = id;
  _obj.backgroundColor = -1;
  _obj.elevation = 20;
  _obj.imageName = "";
  _obj.imageRotation = 0.0;
}

std::pair<int, int> Square::getPosition() const {
  return _obj.position;
}

void Square::setPosition(const std::pair<int, int> &pos) {
  _obj.position = pos;
}

std::pair<int, int> Square::getSize() const {
  return _obj.size;
}

void Square::setSize(const std::pair<int, int> &size) {
  _obj.size = size;
}

std::string Square::getImage() const {
  return _obj.imageName;
}

void Square::setImage(const std::string &img) {
  _obj.imageName = img;
}

int Square::getBackgroundColor() const {
  return _obj.backgroundColor;
}

void Square::setBackgroundColor(const int &bg) {
  _obj.backgroundColor = bg;
}

int Square::getRotation() const {
  return _obj.imageRotation;
}

void Square::setRotation(const int &rotation) {
  _obj.imageRotation = rotation;
}

Object Square::render() {
  return _obj;
}
