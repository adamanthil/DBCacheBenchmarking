#ifndef TUPLE_H_
#define TUPLE_H_

#include <iostream>

#include "Attribute.h"
#include "Schema.h"
#include "globals.h"


class Tuple
{
 private:
  byte * m_data;
  const Schema * m_schema;
 public:

  Tuple();

  void schema(const Schema * schema);
  void data(byte * values);

  void value(byte * buffer, int fid) const;
  void value(int * buffer, int fid) const;
  void value(int * buffer, const Attribute & attribute) const;

  void dump(std::ostream & strm, char fs = '|', char rs = '\n');
};

#endif
