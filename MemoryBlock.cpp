#include <new>
#include <stdio.h>
#include <string.h>

#include "MemoryBlock.h"

MemoryBlock::MemoryBlock(size_t capacity) :
  m_capacity(capacity), m_size(0)
{
  m_buffer = new byte[m_capacity];
  memset(m_buffer, 0, sizeof(m_buffer));
}

MemoryBlock::MemoryBlock(byte * buffer, size_t length) :
  m_capacity(length)
{ 
  m_buffer = new byte[m_capacity];
  memset(m_buffer, 0, sizeof(m_buffer));
  memcpy(m_buffer, buffer, length);
}

MemoryBlock::~MemoryBlock()
{
  delete [] m_buffer;
  m_capacity = 0;
  m_size = 0;
}

size_t MemoryBlock::getSize(void) const
{
  return m_size;
}

void MemoryBlock::setSize(size_t sz) 
{
  m_size = sz;
}

size_t MemoryBlock::capacity() const
{
  return m_capacity;
}

void MemoryBlock::copy(const MemoryBlock & b)
{
  // TODO: sanity checks. 
  memcpy(m_buffer, b.m_buffer, m_capacity);
  m_size = b.m_size;
}

void MemoryBlock::get(byte * buffer, size_t offset, size_t nbytes)
{
  copy(buffer, m_buffer + offset, nbytes);
};

size_t MemoryBlock::put(byte * buffer, size_t offset, size_t nbytes)
{
  copy(m_buffer + offset, buffer, nbytes); 
  return offset + nbytes;
}

void MemoryBlock::copy(byte * dst, byte * src, size_t nbytes)
{
  memcpy(dst, src, nbytes);
}

void MemoryBlock::clear()
{
  m_size = 0;
  memset(m_buffer, 0, m_capacity);
}
