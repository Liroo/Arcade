#ifndef SCORE_H
# define SCORE_H

# include <vector>
# include <utility>

# define WRN_SCORE "Score failed to load, there will be no score"

std::string getBetterScoresForGame(const std::string&);
std::string getPersonalScoreForGame(const std::string&, const std::string&);
void setScoreForGame(const std::string&, const std::string&, int);

#endif
