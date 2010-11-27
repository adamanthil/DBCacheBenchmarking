#ifndef MEMORY_BUFFER_H_
#define MEMORY_BUFFER_H_

#include <cstring>

typedef unsigned char byte;

class MemoryBuffer
{
 private:
  byte * m_buffer;
  size_t m_capacity;
  size_t m_size;
 public:
  MemoryBuffer(size_t capacity);
  MemoryBuffer(byte * buffer, size_t length);
  ~MemoryBuffer();

  void get(byte * buffer, size_t offset, size_t nbytes);
  size_t put(byte * buffer, size_t offset, size_t nbytes);
  void clear();
};

#endif
