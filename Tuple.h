#ifndef TUPLE_H_
#define TUPLE_H_

#include "Attribute.h"
#include "Schema.h"

class Tuple
{
 private:
  char * m_data;
  Schema * m_schema;
 public:
  void value(int fid);
  void value(std::string field);
  void value(Attribute & attribute);
};

#endif
