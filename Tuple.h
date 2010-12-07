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

  byte * m_data;

  void schema(const Schema * schema);
  const Schema * schema() const;

  void value(int * buffer, const std::string & column) const;
  void value(char * buffer, const std::string & column) const;

  void value(int * buffer, const Attribute & attribute) const;
  void value(char * buffer, const Attribute & attribute) const;

  void map(const Tuple * other);
  //void extract(byte * data, ProjectionList & items);

  void dump(std::ostream & strm, char fs = '|', char rs = '\n') const;
 private:
  void value(void * buffer, const Attribute & attribute) const;
  void value(void * buffer, const std::string & column) const;
};

#endif
