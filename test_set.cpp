#include <assert.h>
#include <cstdio>

#include <list>
#include <string>
#include <vector>

#include "./test_set.h"

namespace TestSet {

void ReadDescriptors(const std::string &descriptorsPath,
                     std::list<Descriptor> *descriptors) {
  std::FILE *descriptorsFile = std::fopen(descriptorsPath.c_str(), "r");
  if (descriptorsFile == 0) {
    std::fprintf(stderr, "%s could not be loaded.\n", descriptorsPath.c_str());
    return;
  }

  std::string parentDirectory;
  GetDirectory(descriptorsPath, &parentDirectory);

  while (!feof(descriptorsFile)) {
    char pathBuffer[1000];
    std::fscanf(descriptorsFile, "%s", pathBuffer);
    if (*pathBuffer == '#') {
      std::fscanf(descriptorsFile, "%*[^\n]\n");
      continue;
    }
    std::string path(pathBuffer);

    std::string directory, name, extention;
    SplitPath(path, &directory, &name, &extention);

    if (extention == "bmp"
        || extention == "jpg"
        || extention == "png") {
      Descriptor descriptor(parentDirectory + '/' + path);
      descriptor.Read(descriptorsFile);
      descriptors->push_back(descriptor);
    } else if (extention == "txt") {
      fscanf(descriptorsFile, "\n");
      ReadDescriptors(parentDirectory + '/' + path, descriptors);
    } else {
      std::fprintf(stderr, "Unknown type: %s.",
                   (parentDirectory + '/' + path).c_str());
    }
  }
  std::fclose(descriptorsFile);
}

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

double GetError(const std::vector< std::vector<size_t> > &errors,
                double alphaErrorWeight,
                double betaErrorWeight) {
  size_t experiments = errors[0][0] + errors[0][1] +
    errors[1][0] + errors[1][1];
  double error = (static_cast<double>(errors[0][1]) * betaErrorWeight +
                  static_cast<double>(errors[1][0]) * alphaErrorWeight)
    / experiments;
  return error;
}
}
