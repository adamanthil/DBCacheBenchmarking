#ifndef SCHEMA_H_
#define SCHEMA_H_

#include <vector>
#include <cstring>

#include "Attribute.h"

class Schema : private std::vector<Attribute *>
{
 private:
  size_t m_size;
  std::vector<int> m_offset;
 public:
  Schema();
  ~Schema();
  void add(const Attribute * attribute);
  size_t rsize() const;
  size_t nitems() const;
  int offset(const Attribute * attribute) const;
  int offset(int fpid) const;
  const Attribute * operator[](const std::string &) const;
  const Attribute * operator[](int fid) const;
  const Attribute * at(int fid) const;
};

#endif
