#ifndef BUFFER_MANAGER_H_
#define BUFFER_MANAGER_H_

#include <cstring>

#include "FileDescriptor.h"
#include "DiskPage.h"

#define DFLT_BLOCK_SIZE 4096

class BufferManager
{
private:
  size_t m_blockSize;

  BufferManager(size_t blockSize = DFLT_BLOCK_SIZE);
  ~BufferManager();

  static BufferManager * instance;
public:
  static void Initialize(size_t blockSize = DFLT_BLOCK_SIZE);
  static BufferManager * getInstance();

  DiskPage * read(FileDescriptor * fd);
  MemoryBlock * allocate();
  MemoryBlock * allocate(size_t nblocks);
  void deallocate(MemoryBlock * block);
  
};

#endif
  
