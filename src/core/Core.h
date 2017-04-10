#ifndef CORE_H
# define CORE_H

# include <string>
# include <vector>
# include <memory>
# include "AGames.h"
# include "IGraphic.h"
# include "Event.h"
# include "DirectoryReader.h"
# include "Object.h"
# include "LibLoader.h"

# define DEBUG_GAME "./games/libGameTest.so"

# define ERR_STARTEDLIB "This lib is not compatible or does not exist: "
# define ERR_NOMORELIBAVAILABLE "No more lib available, exit"
# define ERR_NOMOREGAMEAVAILABLE "No more game available, exit"
# define ERR_MENUNOTAVAILABLE "Menu is not available anymore, exit"
# define WRN_LIBNOTAVAILABLE "This lib is not available anymore, refresh"
# define WRN_GAMENOTAVAILABLE "This game is not available anymore, refresh"
# define UNK_EVNT "Unknown event called"

# define MENU_PATH "./games/menu/libmenu.so"

namespace Arcade {
  class Core {
  public:
    Core();
    ~Core();

  private:
    int _loadGame();
    int _loadLib();
    void _runGame();
    int _runLib();

  private:
    void _iterateIndex(const DirectoryReader::DirectoryContent&,
      int&, int);
    DirectoryReader::DirectoryContent _availableLib;
    int _availableLibIndex = -1;
    DirectoryReader::DirectoryContent _availableGame;
    int _availableGameIndex = -1;

  public:
    bool init(const std::string &);
    int run();

  private:
    // handling for event, key pressed, resize...
    void _handleEvent(const Event&);
    void _handleTick();
    void _handleKey(const Event&);
    void _handleResize();

    // Core event
    void _changeLib(int offset);
    void _changeGame(int offset);
    void _restartGame();
    void _startMenu();
    void _playMenu();
    void _exit();

  private:
    // private library game and graphic
    bool _isMenu = true; // game is a menu
    std::unique_ptr<LibLoader<AGames>> _dlGame;
    std::unique_ptr<AGames> _game;
    std::unique_ptr<LibLoader<IGraphic>> _dlGraphic;
    std::unique_ptr<IGraphic> _graphic;

  private:
    bool _isRunning = true;
  };
};

#endif
