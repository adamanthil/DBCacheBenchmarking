#ifndef PROJECTION_H_
#define PROJECTION_H_

#include <iostream>

#include "IRelationalOperator.h"
#include "Schema.h"
#include "Tuple.h"

class Projection : public IRelationalOperator
{
 private:
  MemoryBlock * m_buffer;
  Schema * m_schema;
  Tuple m_tuple;
  IRelationalOperator & m_child;
 public:
  Projection(IRelationalOperator & op, Schema & schema);
  virtual bool moveNext();
  virtual void next(MemoryBlock &);
  virtual void reset() {}
  
  virtual void dump(std::ostream & strm,
		    char fs = '|', char rs = '\n');
};

#endif
