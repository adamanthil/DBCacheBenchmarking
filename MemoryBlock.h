#ifndef MEMORY_BLOCK_H_
#define MEMORY_BLOCK_H_

#include <cstring>

#include "globals.h"

class MemoryBlock
{
 private:
  byte * m_buffer;
  size_t m_capacity;
  size_t m_size;

  void copy(byte * dst, byte * src, size_t nbytes);
 public:
  MemoryBlock(size_t capacity);
  MemoryBlock(byte * buffer, size_t length);
  ~MemoryBlock();

  size_t capacity() const;
  void setSize(size_t sz); 
  size_t getSize(void) const;
  void get(byte * buffer, size_t offset, size_t nbytes);
  size_t put(byte * buffer, size_t offset, size_t nbytes);
  void clear();
  void copy(const MemoryBlock & block);
};

#endif
