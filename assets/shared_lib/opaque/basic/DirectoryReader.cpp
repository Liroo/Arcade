#include "DirectoryReader.h"

/*
  This custom algo is O(n) + O(k) speed (didn't found a fastest way to do what I want)
    ~ anyway O(n) + O(k) is actually pretty fast

  where:
    - n is the numbers of file already stored
    - k is the numbers of file in directory
*/

bool DirectoryReader::updateDirectoryContent(const char* directoryName,
    DirectoryReader::DirectoryContent& data) {
  DIR *dir;

  if ((dir = opendir(directoryName)) == NULL) {
    // ERROR
    std::cerr << ERR_READDIR << directoryName << std::endl;
    return false;
  }

  struct dirent *ent;
  struct stat fileStat;
  std::unordered_set<std::string> regularFiles;
  std::string relativeFilePath;

  // store all the files within directory
  while ((ent = readdir(dir)) != NULL) {
    if (ent->d_name[0] == '.') {
      continue;
    }
    relativeFilePath = std::string(directoryName) + ent->d_name;
    stat(relativeFilePath.c_str(), &fileStat);
    if (!S_ISREG(fileStat.st_mode)) {
      continue;
    }
    regularFiles.insert(relativeFilePath);
    if (!std::binary_search(data.begin(), data.end(), relativeFilePath)) {
      data.push_back(relativeFilePath);
    }
  }
  closedir(dir);
  // custom for_each getting iterator as value
  // useful to clear memory of old library deleted during runtime
  auto i = data.begin();
  while (i < data.end()) {
    if (regularFiles.find(*i) == regularFiles.end()) {
      i = data.erase(i);
    } else {
      ++i;
    }
  }
  return true;
}
