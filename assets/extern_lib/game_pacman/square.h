#ifndef SQUARE_H
# define SQUARE_H

#include <string>

namespace Arcade {
  class Square {

  private:
    std::pair<int, int> _pos;
    std::pair<int, int> _size;
    int _bg;
    int _id;
    int _elevation;
    int _rotation;
    std::string _imageName;

  public:
    Square(const int &id);
    Square(const std::pair<int, int> &pos, const std::pair<int, int> &size, const int &bg, const std::string& img);
    virtual ~Square() {};

    std::pair<int, int> getPosition() const;
    void setPosition(const std::pair<int, int>&);
    std::pair<int, int> getSize() const;
    void setSize(const std::pair<int, int>&);
    std::string getImage() const;
    void setImage(const std::string&);
    int getBackgroundColor() const;
    void setBackgroundColor(const int&);
    int getRotation() const;
    void setRotation(const int&);
    Object render();

  private:
    Object _wallObj;

  };
};

#endif
