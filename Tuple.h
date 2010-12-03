#ifndef TUPLE_H_
#define TUPLE_H_

#include <iostream>

#include "Attribute.h"
#include "Schema.h"
#include "globals.h"


class Tuple
{
 private:
  const Schema * m_schema;
 public:

  Tuple();

  const byte * m_data;

  void schema(const Schema * schema);
  const Schema * schema() const;

  void value(int * buffer, const Attribute & attribute) const;
  void value(char * buffer, const Attribute & attribute) const;

  void dump(std::ostream & strm, char fs = '|', char rs = '\n');
 private:
  void value(void * buffer, size_t length, const Attribute & attribute) const;
};

#endif
