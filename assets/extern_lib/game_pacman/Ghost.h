#ifndef GHOST_H
# define GHOST_H

# include "Object.h"

# define DIR_RESSOURCES "assets/ressource/"

namespace Arcade {

  typedef enum e_directionType {
    DIRECTION_UP,
    DIRECTION_DOWN,
    DIRECTION_RIGHT,
    DIRECTION_LEFT,

    DIRECTION_DEFAULT = -1,
  } DirectionType;

  class Ghost {
  private:
    std::pair<int, int> _position;
    std::pair<int, int> _rawPosition;
    std::pair<int, int> _size;
    DirectionType _direction;
    std::string _id;
    char _inMap;
    int _elevation;
    std::string _imageName;
    Object _object;

  public:
    Ghost(const std::string&, const std::string&);
    ~Ghost() {};
    Ghost &operator=(const Ghost&);
    std::pair<int, int> getPosition() const;
    void setPosition(const std::pair<int, int>&);
    std::pair<int, int> getSize() const;
    void setSize(const std::pair<int, int>&);
    std::string getImageName() const;
    void setImageName(const std::string&);
    std::string getId() const;
    char getInMap() const;
    void setInMap(const char &);
    int getDirection() const;
    void setDirection(const DirectionType &);
    Object render();
    std::pair<int, int> getRawPosition() const;
    void setRawPosition(const std::pair<int, int>&);

  };
};

#endif
