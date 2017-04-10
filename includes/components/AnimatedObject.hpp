#ifndef ANIMATEDOBJECT_HPP
# define ANIMATEDOBJECT_HPP

# include <algorithm>
# include "DirectoryReader.h"
# include "Object.h"

namespace Arcade {
  class AnimatedObject {
    public:
      AnimatedObject(const std::string&,
        const std::string&);
      // size, position, id, directoryPath
      AnimatedObject(const std::pair<int, int>&,
        const std::pair<int, int>&,
        const std::string&,
        const std::string&);
      ~AnimatedObject();

    public:
      bool init();

    private:
      std::string _directoryPath;
      DirectoryReader::DirectoryContent _images;
      int _imagesIndex;

    private:
      Object _image;

    public:
      void setSize(const std::pair<int, int>&);
      void setPosition(const std::pair<int, int>&);

    public:
      Object render();
      Object renderNext();

  };
};

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

Object AnimatedObject::renderNext() {
  _imagesIndex++;
  if (_imagesIndex >= (int)_images.size()) {
    _imagesIndex = 0;
  }
  _image.imageName = _images[_imagesIndex];
  return _image;
}

#endif
