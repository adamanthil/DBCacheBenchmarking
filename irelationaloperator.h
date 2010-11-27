#ifndef I_RELATIONAL_OPERATOR_H_
#define I_RELATIONAL_OPERATOR_H_

#include "memorybuffer.h"
#include "schema.h"

class IRelationalOperator
{
 public:
  virtual boolean moveNext() = 0;
  virtual void next(MemoryBuffer & buffer) = 0;
  virtual const Schema & schema() = 0;

  /*
  virtual void open() = 0;
  virtual void reset() = 0;
  virtual void close() = 0;
  */
};

#endif
