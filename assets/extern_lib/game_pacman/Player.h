#ifndef PLAYER_H
# define PLAYER_H

#include "Ghost.h"

# define DIR_RESSOURCES "assets/ressource/"

namespace Arcade {

  class Player {
  private:
    std::pair<int, int> _position;
    std::pair<int, int> _size;
    std::pair<int, int> _rawPosition;
    std::vector<std::pair<int, int>> _cellToPass;
    char _inMap;
    DirectionType _direction;
    std::string _id;
    int _elevation;
    std::string _imageName;
    float _rotation;
    Object _object;

  public:
    Player(const std::string&, const std::string&);
    ~Player() {};
    std::pair<int, int> getPosition() const;
    void setPosition(const std::pair<int, int>&);
    std::pair<int, int> getSize() const;
    void setSize(const std::pair<int, int>&);
    std::string getImageName() const;
    void setImageName(const std::string&);
    char getInMap() const;
    void setInMap(const char &);
    int getDirection() const;
    void setDirection(const DirectionType &);
    std::string getId() const;
    void setId(const std::string&);
    float getRotation() const;
    void setRotation(const int &);
    Object render();
    std::pair<int, int> getRawPosition() const;
    void setRawPosition(const std::pair<int, int>&);

  };
};

#endif
