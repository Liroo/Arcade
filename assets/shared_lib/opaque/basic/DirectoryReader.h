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

    static bool updateDirectoryContent(const char* directoryName,
        DirectoryContent& data);
};

#endif
