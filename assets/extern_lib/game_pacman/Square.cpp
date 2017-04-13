#include "Square.h"

using namespace Arcade;

Square::Square(const std::string &id) {
  _object.id = id;
  _object.backgroundColor = -1;
  _object.elevation = 20;
  _object.imageName = "";
  _object.size = {30, 30};
  _object.imageRotation = 0.0;
  _object.rawImage = {};
}

Square::Square(const std::pair<int, int> &pos, const std::pair<int, int> &size, const std::string &id) {
  _object.position = pos;
  _object.size = size;
  _object.id = id;
  _object.backgroundColor = -1;
  _object.elevation = 20;
  _object.imageName = "";
  _object.imageRotation = 0.0;
  _object.rawImage = {};
}

std::pair<int, int> Square::getPosition() const {
  return _object.position;
}

void Square::setPosition(const std::pair<int, int> &pos) {
  _object.position = pos;
}

std::pair<int, int> Square::getSize() const {
  return _object.size;
}

void Square::setSize(const std::pair<int, int> &size) {
  _object.size = size;
}

std::string Square::getImage() const {
  return _object.imageName;
}

void Square::setImage(const std::string &img) {
  _object.imageName = img;
}

int Square::getBackgroundColor() const {
  return _object.backgroundColor;
}

void Square::setBackgroundColor(const int &bg) {
  _object.backgroundColor = bg;
}

int Square::getRotation() const {
  return _object.imageRotation;
}

void Square::setRotation(const int &rotation) {
  _object.imageRotation = rotation;
}

std::vector<std::string> Square::getRawImage() const {
  return _object.rawImage;
}

void Square::setRawImage(const std::vector<std::string> &rawImage) {
  _object.rawImage = rawImage;
}

Object Square::render() {
  return _object;
}
