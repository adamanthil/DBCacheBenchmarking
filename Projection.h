#ifndef PROJECTION_H_
#define PROJECTION_H_

#include <iostream>

#include "IRelationalOperator.h"
#include "Tuple.h"

class Projection : public IRelationalOperator
{
 private:
  MemoryBlock * m_buffer;
  Schema * m_schema;
  IRelationalOperator & m_child;
 public:
  Projection(IRelationalOperator & op,
	     std::vector<Attribute *> & attributes);
  virtual bool moveNext();
  virtual void next(MemoryBlock &);
  virtual void reset() {}
  //virtual const Schema & schema();
  
  virtual void dump(std::ostream & strm,
		    char fs = '|', char rs = '\n');
};

#endif
