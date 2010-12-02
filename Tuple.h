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
 public:

  Schema * m_schema;
  void data(byte * values);

  void value(byte * buffer, int fid);
  void value(int * buffer, int fid);

  void dump(std::ostream & strm, char fs = '|', char rs = '\n');
};

#endif
