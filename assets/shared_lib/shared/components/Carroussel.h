#ifndef CARROUSSEL_HPP
# define CARROUSSEL_HPP

# include "Button.h"

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

#endif
