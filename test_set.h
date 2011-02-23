#ifndef _TEST_SET_H_
#define _TEST_SET_H_

#include <cstdio>


#include <list>
#include <string>
#include <fstream>

namespace TestSet {

/*! \struct Descriptor
  \brief Test image descriptor. See risk factors
  table in docs folder.
*/
struct Descriptor {
  std::string imagePath;
  double polypoidFormation;

  Descriptor(const std::string &path)
    : imagePath(path) {}

  void Read(std::FILE *file) {
    std::fscanf(file, "%*s %lf\n", &polypoidFormation);
  }

  void Print() {
    std::printf("%s   polyp: %lf\n", imagePath.c_str(),
               polypoidFormation);
  }
};


/*!
 \brief Reads file that describes test collection.
 Example:
   image001.png   polyp 1.0
   image002.png   polyp 0.9
   test/description.txt   ---  will be called recursively
 \param descriptorsPath Path to root description.
 \param Pointer to resulting list of descriptors. 
*/
 void ReadDescriptors(const std::string &descriptorsPath,
                      std::list<Descriptor> *descriptors);

/*!
 \brief Gets directory part from path.
 Example: a/b/v.txt -> a/b
 \param path Path itself.
 \param directory Pointer to directory that was taken out.
*/
void GetDirectory(const std::string &path, std::string *directory);

/*!
 \brief Splits path into directory part, file name and extension.
 Example: a/b/v.txt -> a/b v txt
 \param path Path itself.
 \param directory Pointer to directory that was taken out.
 \param name Pointer to name that was taken out.
 \param extension Pointer to extension that was taken out.
*/
void SplitPath(const std::string &path,
               std::string *directory, std::string *name,
               std::string *extension);

}

#endif /* _TEST_SET_H_ */
