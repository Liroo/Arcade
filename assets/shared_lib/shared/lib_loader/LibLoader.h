#ifndef LIBLOADER_H
# define LIBLOADER_H

# include <string>
# include "dlfcn.h"

# define DFL_ENTRY_NAME "load_lib"

template <typename LibType>
class LibLoader {
public:
  LibLoader(const std::string &entryName) {
      this->_entryName = entryName;
      this->_handle = nullptr;
  }

  ~LibLoader() {}

private:
  void *_handle;
  std::string _entryName;
  bool _isOpen = false;

public:
  LibType* openLib(const std::string &libName) {
    if (_handle != NULL && !closeLib()) {
      return NULL;
    }
    // WARNING behavior of this define (RTLD_LAZY) is not well known
    _handle = dlopen(libName.c_str(), RTLD_LAZY);
    if (_handle == NULL) {
      std::cerr << dlerror() << std::endl;
      return NULL;
    }
    LibType* (*external_creator)();
    external_creator = (LibType * (*)())(dlsym(_handle, _entryName.c_str()));
    if (external_creator == NULL) {
      std::cerr << dlerror() << std::endl;
      return NULL;
    }
    _isOpen = true;
    return external_creator();
  }

  bool closeLib() {
    _isOpen = false;
    if (dlclose(_handle) != 0) {
      std::cerr << dlerror() << std::endl;
      _handle = NULL;
      return false;
    }
    _handle = NULL;
    return true;
  }

  bool isOpen() {
    return _isOpen;
  }
};

#endif
