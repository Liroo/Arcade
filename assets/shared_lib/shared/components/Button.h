#ifndef BUTTON_HPP
# define BUTTON_HPP

# include "Object.h"

// Button elevation [10, 20[

namespace Arcade {
  class Button {
    public:
      Button() {};
      Button(const std::string&);
      // size, position, id
      Button(const std::pair<int, int>&,
        const std::pair<int, int>&,
        const std::string&);
      ~Button();

    private:
      std::pair<int, int> _size;
      std::pair<int, int> _position;
      int _backgroundColor;
      int _focusColor;
      int _shadowColor;
      int _fontSize;
      std::string _text;
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
      int getFontSize() const;
      void setFontSize(int);
      std::string getText() const;
      void setText(const std::string&);

    public:
      ObjectList render();
      ObjectList renderFocus();
      ObjectList renderPressed();

    private:
      Object _background;
      Object _shadow;
  };
};

#endif
