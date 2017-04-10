#ifndef ARCADEEXCEPTION_HPP
# define ARCADEEXCEPTION_HPP

# include <exception>
# include <string>

# define ERR_GRAPHICINIT "Graphic lib failed to init"
# define ERR_MENUNOTAVAILABLEBECAUSELIB "Menu is not available anymore because there is missing library, exit"

namespace Arcade {
  namespace Exception {
    class ArcadeException: public std::exception {
      public:
        ArcadeException(const std::string &msg): _msg(msg) {};
        virtual ~ArcadeException() {};

      protected:
        std::string _msg;

      public:
        const char *what() const throw() { return _msg.c_str(); };
    };
  };
};

#endif
