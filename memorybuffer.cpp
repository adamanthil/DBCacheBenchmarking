#include "memorybuffer.h"

MemoryBuffer::MemoryBuffer(size_t capacity) :
  m_capacity(capacity)
{
  m_buffer = new byte[m_capacity];
}

MemoryBuffer::MemoryBuffer(byte * buffer, size_t length) :
  m_capacity(length)
{
  /*
  m_buffer = new byte[m_capacity];
  memcpy(m_buffer, buffer, );
  */
}

MemoryBuffer::~MemoryBuffer()
{
  delete [] m_buffer;
}
