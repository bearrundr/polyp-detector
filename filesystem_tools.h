#ifndef _FILESYSTEM_TOOLS_H_
#define _FILESYSTEM_TOOLS_H_

#include <string>

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


#endif /* _FILESYSTEM_TOOLS_H_ */
