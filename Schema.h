#ifndef SCHEMA_H_
#define SCHEMA_H_

#include <map>
#include <vector>
#include <cstring>

#include "Attribute.h"

class Schema : private std::vector<Attribute *>
{
 private:
  size_t m_size;
  std::map<std::string, size_t> m_offset;
  std::string m_relation;
 public:
  Schema();
  Schema(const std::vector<const Attribute *> * columns);
  ~Schema();

  void add(const Attribute * attribute);
  size_t rsize() const;
  size_t nitems() const;

  int offset(const Attribute * attribute) const;
  int offset(const std::string & field) const;
  int offset(int fpid) const;

  const Attribute * operator[](const std::string &) const;
  const Attribute * operator[](int fid) const;
  const Attribute * at(int fid) const;

  bool contains(const std::string &) const;

  int m_partitions;
};

#endif
