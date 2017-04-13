#ifndef SCORE_H
# define SCORE_H

# include <vector>
# include <utility>

namespace Arcade {
  class Score {
    public:
      Score() {}
      ~Score() {}

    public:
      static std::vector<std::pair<std::string, int>> getBetterScoresForGame(const std::string&);
      static int getPersonalScoreForGame(const std::string&, const std::string&);
      static void setScoreForGame(const std::string&, const std::string&, int);
  };
};

#endif
