#include <assert.h>

#include <string>

#include "./filesystem_tools.h"

void GetDirectory(const std::string &path, std::string *directory) {
  size_t slashPosition = path.find_last_of('/');
  if (slashPosition == std::string::npos) {
    *directory = ".";
  } else {
    *directory = path.substr(0, slashPosition);
  }
}

void SplitPath(const std::string &path,
               std::string *directory, std::string *name,
               std::string *extension) {
  size_t slashPosition = path.find_last_of('/');
  size_t dotPosition = path.find_last_of('.');

  *directory = ".";
  *name = path;
  *extension = "";

  if (slashPosition == std::string::npos
      && dotPosition == std::string::npos) {
    return;
  } else if (slashPosition == std::string::npos
      && dotPosition != std::string::npos) {
    *name = path.substr(0, dotPosition);
    *extension = path.substr(dotPosition + 1);
  } else if (slashPosition != std::string::npos
             && dotPosition == std::string::npos) {
    *directory = path.substr(0, slashPosition);
    *name = path.substr(slashPosition + 1);
  } else {
    assert(slashPosition < dotPosition);
    *directory = path.substr(0, slashPosition);
    *name = path.substr(slashPosition + 1, dotPosition - slashPosition - 1);
    *extension = path.substr(dotPosition + 1);
  }
}

