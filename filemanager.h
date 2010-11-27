#ifndef FILE_MANAGER_H_
#define FILE_MANAGER_H_

#include <string>
#include "filedescriptor.h"

class FileManager
{
 private:
 public:
  FileDescriptor * open(std::string filename);
  close(FileDescriptor);
};

#endif
