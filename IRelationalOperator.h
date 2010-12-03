#ifndef I_RELATIONAL_OPERATOR_H_
#define I_RELATIONAL_OPERATOR_H_

#include <iostream>
#include <fstream>

#include "MemoryBlock.h"
#include "Schema.h"
#include "Predicate.h"
#include "Tuple.h"

class IRelationalOperator
{
 protected:
  BooleanExpression * m_boolExp;
 public:
  virtual bool moveNext() = 0;
  virtual void next(MemoryBlock & buffer) = 0;
  //virtual const Schema & schema() = 0;

  virtual void reset() = 0;

  virtual bool satisfies();
  virtual void dump(std::ostream & strm,
		    char fs = '|', char rs = '\n') {}

  /*
  virtual void open() = 0;
  virtual void close() = 0;
  */
};

#endif
