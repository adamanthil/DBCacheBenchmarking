#ifndef SCHEMA_H_
#define SCHEMA_H_

#include <vector>
#include <string>
#include <cstring>

#include "attribute.h"

class Schema
{
 private:
 public:
  size_t offset(std::string field);
};

#endif
