#ifndef CARROUSSEL_HPP
# define CARROUSSEL_HPP

# include "components/Button.hpp"

namespace Arcade {
  class Carroussel {
    public:
      Carroussel(const std::string&);
      // size, position, backgroundColor, id
      Carroussel(const std::pair<int, int>&,
        const std::pair<int, int>&,
        const std::string&);
      ~Carroussel();

    private:
      // this is the size of left + 5 + center + 5 + right
      std::pair<int, int> _size;
      std::pair<int, int> _position;
      int _backgroundColor;
      int _focusColor;
      int _shadowColor;
      std::vector<std::string> _textList;
      int _textListIndex;
    public:
      std::pair<int, int> getSize() const;
      void setSize(const std::pair<int, int>&);
      std::pair<int, int> getPosition() const;
      void setPosition(const std::pair<int, int>&);
      int getBackgroundColor() const;
      void setBackgroundColor(int);
      int getFocusColor() const;
      void setFocusColor(int);
      int getShadowColor() const;
      void setShadowColor(int);
      std::vector<std::string> getTextList() const;
      std::string getTextLayout() const;
      void setTextList(std::vector<std::string>);

    public:
      ObjectList render();
      ObjectList renderFocus();
      ObjectList renderPressedLeft();
      ObjectList renderPressedRight();

    private:
      Button _left;
      Button _center;
      Button _right;
  };
};

using namespace Arcade;

Carroussel::Carroussel(const std::string& id):
  _left(id + ":left"),
  _center(id + ":center"),
  _right(id + ":right") {
  _left.setText("<");
  _right.setText(">");
}

Carroussel::Carroussel(const std::pair<int, int>& size,
  const std::pair<int, int>& position,
  const std::string& id):
    _left({
        size.second,
        size.second
      }, position, id + ":left"),
    _center({
      size.first - (2 * size.second) - 10,
      size.second
    }, {
      position.first + size.second + 5,
      size.second
    }, id + ":center"),
    _right({
      size.second,
      size.second
    }, {
      position.first + size.first - size.second,
      size.second
    }, id + ":right")
{
  _size = size;
  _position = position;
  _left.setText("<");
  _right.setText(">");
}

Carroussel::~Carroussel() {}

std::pair<int, int> Carroussel::getSize() const {
  return _size;
}

void Carroussel::setSize(const std::pair<int, int>& size) {
  _left.setSize({
    size.second,
    size.second
  });
  _center.setSize({
    size.first - (2 * size.second) - 10,
    size.second
  });
  _right.setSize({
    size.second,
    size.second
  });
  _left.setPosition(_position);
  _center.setPosition({
    _position.first + size.second + 5,
    _position.second
  });
  _right.setPosition({
    _position.first + size.first - size.second,
    _position.second
  });
  _size = size;
}

std::pair<int, int> Carroussel::getPosition() const {
  return _position;
}

void Carroussel::setPosition(const std::pair<int, int>& position) {
  _left.setSize({
    _size.second,
    _size.second
  });
  _center.setSize({
    _size.first - (2 * _size.second) - 10,
    _size.second
  });
  _right.setSize({
    _size.second,
    _size.second
  });
  _left.setPosition(position);
  _center.setPosition({
    position.first + _size.second + 5,
    position.second
  });
  _right.setPosition({
    position.first + _size.first - _size.second,
    position.second
  });
  _position = position;
}

int Carroussel::getBackgroundColor() const {
  return _backgroundColor;
}

void Carroussel::setBackgroundColor(int backgroundColor) {
  _left.setBackgroundColor(backgroundColor);
  _center.setBackgroundColor(backgroundColor);
  _right.setBackgroundColor(backgroundColor);
  _backgroundColor = backgroundColor;
}

int Carroussel::getFocusColor() const {
  return _focusColor;
}

void Carroussel::setFocusColor(int focusColor) {
  _left.setFocusColor(focusColor);
  _center.setFocusColor(focusColor);
  _right.setFocusColor(focusColor);
  _focusColor = focusColor;
}

int Carroussel::getShadowColor() const {
  return _shadowColor;
}

void Carroussel::setShadowColor(int shadowColor) {
  _left.setShadowColor(shadowColor);
  _center.setShadowColor(shadowColor);
  _right.setShadowColor(shadowColor);
  _shadowColor = shadowColor;
}

std::vector<std::string> Carroussel::getTextList() const {
  return _textList;
}

std::string Carroussel::getTextLayout() const {
  return _textList[_textListIndex];
}

void Carroussel::setTextList(std::vector<std::string> textList) {
  _textListIndex = 0;
  _textList = textList;
}


ObjectList Carroussel::render() {
  _center.setText(_textList[_textListIndex]);

  ObjectList tmp;
  appendListToList(tmp, _left.render());
  appendListToList(tmp, _center.render());
  appendListToList(tmp, _right.render());
  return tmp;
}

ObjectList Carroussel::renderFocus() {
  _center.setText(_textList[_textListIndex]);

  ObjectList tmp;
  appendListToList(tmp, _left.renderFocus());
  appendListToList(tmp, _center.renderFocus());
  appendListToList(tmp, _right.renderFocus());
  return tmp;
}

ObjectList Carroussel::renderPressedLeft() {
  _textListIndex--;
  if (_textListIndex < 0) {
    _textListIndex = _textList.size() - 1;
  }
  _center.setText(_textList[_textListIndex]);

  ObjectList tmp;
  appendListToList(tmp, _left.renderPressed());
  appendListToList(tmp, _center.renderFocus());
  appendListToList(tmp, _right.renderFocus());
  return tmp;
}

ObjectList Carroussel::renderPressedRight() {
  _textListIndex++;
  if (_textListIndex >= (int)_textList.size()) {
    _textListIndex = 0;
  }
  _center.setText(_textList[_textListIndex]);

  ObjectList tmp;
  appendListToList(tmp, _left.renderFocus());
  appendListToList(tmp, _center.renderFocus());
  appendListToList(tmp, _right.renderPressed());
  return tmp;
}

#endif
