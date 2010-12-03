#ifndef SCHEMA_H_
#define SCHEMA_H_

#include <vector>
#include <cstring>

#include "Attribute.h"

class Schema : public std::vector<Attribute *>
{
 private:
  size_t m_size;
 public:
  Schema();
  void add(Attribute * attribute);
  size_t rsize() const;
  size_t nitems() const;
  int offset(const Attribute * attribute) const;
  int offset(int fpos);
};

#endif
