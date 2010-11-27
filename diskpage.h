#ifndef DISK_PAGE_H_
#define DISK_PAGE_H_

#include "pagelayout.h"
#include "memorybuffer.h"

class DiskPage
{
 private:
  const PageLayout & m_layout;
  MemoryBuffer m_buffer;
 public:
  DiskPage(const & PageLayout, size_t capacity);

  size_t size() const;
  size_t capacity() const;
  void get(int rid, const char * field, byte * buffer, size_t length);
  int put(int rid, const char * field, byte * buffer, size_t length);
};

#endif
