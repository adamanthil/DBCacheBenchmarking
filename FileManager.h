#ifndef FILE_MANAGER_H_
#define FILE_MANAGER_H_

#include <string>
#include <vector>

#include "FileDescriptor.h"

class IFileManager
{
 public:
  virtual FileDescriptor * open(const std::string & filename) = 0;
  virtual void close(FileDescriptor *) = 0;
};

class FileManager : public IFileManager
{
 private:
  FileManager();
  ~FileManager();

  std::vector<DiskPage *> m_files[3];
  static FileManager * instance;
 public:

  static void Initialize(const std::string & config_file = "");
  static FileManager * getInstance();

  virtual FileDescriptor * open(const std::string & filename);
  virtual void close(FileDescriptor * fd);
};

#endif
