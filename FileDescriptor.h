#ifndef FILE_DESCRIPTOR_H_
#define FILE_DESCRIPTOR_H_

#include "DiskPage.h"
#include <vector>

class FileDescriptor
{
 private:
  std::vector<DiskPage *> & m_pages;
  int m_pid;
 public:
  FileDescriptor(std::vector<DiskPage *> & pages); 
  bool eof();
  void reset() { m_pid = 0; }
  void seek(int pid);
  int size();
  DiskPage * read();
};

#endif
