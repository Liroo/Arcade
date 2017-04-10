#include "AnimatedObject.h"

using namespace Arcade;

AnimatedObject::AnimatedObject(const std::string& id,
  const std::string& directoryPath) {
  _image.id = id;
  _image.elevation = 2; // elevation background foregroung image
  _directoryPath = directoryPath;
}

AnimatedObject::AnimatedObject(const std::pair<int, int>& size,
  const std::pair<int, int>& position,
  const std::string& id,
  const std::string& directoryPath) {
  _image.size = size;
  _image.position = position;
  _image.id = id;
  _image.elevation = 2; // elevation background foregroung image
  _directoryPath = directoryPath;
}

AnimatedObject::~AnimatedObject() {}

void AnimatedObject::setSize(const std::pair<int, int>& size){
  _image.size = size;
}

void AnimatedObject::setPosition(const std::pair<int, int>& position) {
  _image.position = position;
}

bool AnimatedObject::init() {
  if (!DirectoryReader::updateDirectoryContent(
    _directoryPath.c_str(),
    _images) || _images.empty()) {
    return false;
  }
  std::sort(_images.begin(), _images.end());
  _imagesIndex = 0;
  return true;
}

Object AnimatedObject::render() {
  _image.imageName = _images[_imagesIndex];
  return _image;
}

Object AnimatedObject::renderNext(long timer) {
  _imagesIndex = (timer/100) % _images.size();
  _image.imageName = _images[_imagesIndex];
  return _image;
}
