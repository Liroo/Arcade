#ifndef SQUARE_H
# define SQUARE_H

# include "Object.h"

namespace Arcade {
  class Square {

  public:
    Square(const std::string&);
    Square(const std::pair<int, int>&, const std::pair<int, int>&, const std::string&);
    ~Square() {};

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
    std::vector<std::string> getRawImage() const;
    void setRawImage(const std::vector<std::string>&);
    Object render();

  private:
    Object _object;

  };
};

#endif
