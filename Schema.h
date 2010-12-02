#ifndef SCHEMA_H_
#define SCHEMA_H_

#include <vector>
#include <string>
#include <cstring>

#include "Attribute.h"

class Schema
{
 private:
 public:
  size_t offset(std::string field);
};

#endif
