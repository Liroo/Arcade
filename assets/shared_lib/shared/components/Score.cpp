#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "Score.h"

std::string getBetterScoresForGame(const std::string& game) {
  std::vector<std::string> tmpScore;

  std::ifstream ifs;
  ifs.open("./assets/ressource/.arcade_scores", std::ios::in);
  if (!ifs.is_open()) {
    std::cerr << WRN_SCORE << std::endl;
    return "";
  }
  std::stringstream ss;
  ss << ifs.rdbuf();
  ifs.close();

  std::string item;
  while (std::getline(ss, item, '\n')) {
    if (!item.empty()) {
      tmpScore.push_back(item);
    }
  }


  std::vector<std::pair<std::string, int>> gameScore;
  for (auto it = tmpScore.begin(); it < tmpScore.end(); it++) {
    if ((*it).size() >= game.size() && (*it).compare(0, game.size(), game) == 0) {
      std::string score = (*it).substr(game.size() + 1);
      int idx = score.find(':');
      gameScore.push_back(std::pair<std::string, int>({
        score.substr(0, idx),
        std::stoi(score.substr(idx + 1))
      }));
    }
  }
  std::stable_sort(gameScore.begin(), gameScore.end(),
    [](std::pair<std::string, int> i1, std::pair<std::string, int> i2) -> bool {
      return i1.second > i2.second;
    });

  std::string finalScore;
  int i = 0;
  while (i < (int)gameScore.size() && i < 5) {
    finalScore += gameScore[i].first + " : " + std::to_string(gameScore[i].second) + "\n";
    i++;
  }
  while (i < 5) {
    finalScore += "0\n";
    i++;
  }

  return finalScore;
}

std::string getPersonalScoreForGame(const std::string& game, const std::string& pseudo) {
  std::vector<std::string> tmpScore;

  std::ifstream ifs;
  ifs.open("./assets/ressource/.arcade_scores", std::ios::in);
  if (!ifs.is_open()) {
    std::cerr << WRN_SCORE << std::endl;
    return "";
  }
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
      return i1 > i2;
    });
  if (playerScore.size() > 0) {
    return pseudo + ":" + std::to_string(playerScore[0]);
  }
  return "";
}

void setScoreForGame(const std::string& game, const std::string& pseudo, int score) {
  std::ofstream out("./assets/ressource/.arcade_scores", std::ios_base::app);
  std::string finalScore;
  finalScore = game + ":" + pseudo + ":" + std::to_string(score);
  out << finalScore << std::endl;
  out.close();
  return;
}
