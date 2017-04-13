#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "Score.h"

using namespace Arcade;

std::vector<std::pair<std::string, int>> Arcade::Score::getBetterScoresForGame(const std::string& game) {
  std::vector<std::string> tmpScore;

  std::ifstream ifs("./ressource/.arcade_scores");
  std::stringstream ss;
  ss << ifs.rdbuf();
  ifs.close();

  std::string item;
  while (std::getline(ss, item, '\n')) {
      tmpScore.push_back(item);
  }

  std::vector<std::pair<std::string, int>> gameScore;
  for (auto it = tmpScore.begin(); it < tmpScore.end(); it++) {
    if ((*it).compare(0, game.length(), game) == 0) {
      std::string score = (*it).substr(game.size() + 1);
      int idx = score.find(':');
      gameScore.push_back({
        score.substr(0, idx - 1),
        std::stoi(score.substr(idx + 1))
      });
    }
  }
  std::stable_sort(gameScore.begin(), gameScore.end(),
    [](std::pair<std::string, int> i1, std::pair<std::string, int> i2) -> bool {
      return i1.second < i2.second;
    });
  if (gameScore.size() > 5) {
    gameScore.erase(gameScore.begin() + 5, gameScore.end());
  }
  return gameScore;
}

int Arcade::Score::getPersonalScoreForGame(const std::string& game, const std::string& pseudo) {
  std::vector<std::string> tmpScore;

  std::ifstream ifs("./ressource/.arcade_scores");
  std::stringstream ss;
  ss << ifs.rdbuf();
  ifs.close();

  std::string item;
  while (std::getline(ss, item, '\n')) {
    if (!item.empty()) {
      tmpScore.push_back(item);
    }
  }

  std::string beginStr = game + ":" + pseudo + ":";
  std::vector<int> playerScore;
  for (auto it = tmpScore.begin(); it < tmpScore.end(); it++) {
    if ((*it).compare(0, beginStr.length(), beginStr) == 0) {
      std::string score = (*it).substr(beginStr.size());
      playerScore.push_back(std::stoi(score));
    }
  }
  std::stable_sort(playerScore.begin(), playerScore.end(),
    [](int i1, int i2) -> bool {
      return i1 < i2;
    });
  if (playerScore.size() > 0) {
    return playerScore[0];
  }
  return 0;
}

void Arcade::Score::setScoreForGame(const std::string& game, const std::string& pseudo, int score) {
  std::ofstream out("./ressource/.arcade_scores");
  std::string finalScore;
  finalScore = game + ":" + pseudo + ":" + std::to_string(score);
  out << finalScore << std::endl;
  out.close();
  return;
}
