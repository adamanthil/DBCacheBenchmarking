#ifndef FILE_DESCRIPTOR_H_
#define FILE_DESCRIPTOR_H_

#include "diskpage.h"

class FileDescriptor
{
 private:
 public:
  boolean eof();
  void reset();
  void seek(int pid);
  DiskPage * read();
};

#endif
