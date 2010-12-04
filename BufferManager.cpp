#include <new>
#include "BufferManager.h"

BufferManager * BufferManager::instance = 0;

BufferManager::BufferManager(size_t blockSize) :
  m_blockSize(blockSize)
{
}

BufferManager::~BufferManager()
{
}

void BufferManager::Initialize(size_t blockSize)
{
  if (!BufferManager::instance)
    {
      BufferManager::instance = new BufferManager(blockSize);
    }
}

BufferManager * BufferManager::getInstance()
{
  if (BufferManager::instance == NULL)
    {
      BufferManager::Initialize();
    }

  return BufferManager::instance;
}

DiskPage * BufferManager::read(FileDescriptor * fd)
{
  return fd->read();
}

MemoryBlock * BufferManager::allocate()
{
  return new MemoryBlock(m_blockSize);
}

MemoryBlock * BufferManager::allocate(size_t nblocks)
{
  MemoryBlock * blocks = NULL;
  return blocks;
}

void  BufferManager::deallocate(MemoryBlock * block)
{
  delete block;
}


