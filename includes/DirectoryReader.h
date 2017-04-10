#ifndef DIRECTORY_READER
# define DIRECTORY_READER

# include <string>
# include <vector>
# include <iostream>
# include <algorithm>
# include <unordered_set>
// C includes
# include "dirent.h"
# include "sys/stat.h"

# define ERR_READDIR "There is an error during reading of directory: "

# define DIR_LIB "lib/"
# define DIR_GAMES "games/"
# define DIR_RESSOURCES "assets/ressource/"

class DirectoryReader {
  public:
    typedef std::vector<std::string> DirectoryContent;
    /*
      This custom algo is O(n) + O(k) speed (didn't found a fastest way to do what I want)
        ~ anyway O(n) + O(k) is actually pretty fast

      where:
        - n is the numbers of file already stored
        - k is the numbers of file in directory
    */
    static bool updateDirectoryContent(const char* directoryName,
        DirectoryContent& data) {
      DIR *dir;

      if ((dir = opendir(directoryName)) == NULL) {
        // ERROR
        std::cerr << ERR_READDIR << directoryName << std::endl;
        return false;
      }

      struct dirent *ent;
      struct stat fileStat;
      std::unordered_set<std::string> regularFiles;

      // store all the files within directory
      while ((ent = readdir(dir)) != NULL) {
        if (ent->d_name[0] == '.') {
          continue;
        }
        std::string relativeFilePath = std::string(directoryName) + ent->d_name;
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
      for (auto i = data.begin(); i < data.begin(); i++) {
        if (!std::binary_search(regularFiles.begin(), regularFiles.end(), *i)) {
          data.erase(i);
        }
      }
      return true;
    }
};

#endif
