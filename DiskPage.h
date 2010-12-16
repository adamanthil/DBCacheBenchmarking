#ifndef DISK_PAGE_H_
#define DISK_PAGE_H_

#include <vector>
#include <string>

#include "PageLayout.h"
#include "MemoryBlock.h"
#include "Schema.h"

typedef std::vector<Attribute *> AttributeList;

class DiskPage
{
 private:
  const PageLayout * m_layout;
  MemoryBlock * m_block;
  std::string m_table;
 public:
  DiskPage(const PageLayout * layout, MemoryBlock * block,
	   const std::string & table);

  size_t size() const;
  size_t capacity() const;
  void get(int rid, const Schema * fields, byte * buffer, size_t length);
  void get(int rid, const std::vector<const Attribute *> & fields, byte * buffer, size_t length);
  //void get(int rid, const AttributeList & fields, byte * buffer, size_t length);
  int put(int rid, const char * field, char * buffer, size_t length);
};

#endif
