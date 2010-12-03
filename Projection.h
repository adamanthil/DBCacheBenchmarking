#ifndef PROJECTION_H_
#define PROJECTION_H_

#include <iostream>

#include "IRelationalOperator.h"
#include "Schema.h"
#include "Tuple.h"

class Projection : public IRelationalOperator
{
 private:
  enum { IN = 0, OUT = 1 };

  /** minimum sace required to store tuple. */
  int m_rsize;
  /** flag to check if all data from current buffer has been consumed. */
  bool m_consumed; 
  /** next record id to consume. */
  int m_next; 

  MemoryBlock * m_buffer[2];
  Tuple m_tuple[2];
  IRelationalOperator & m_child;
 public:
  Projection(IRelationalOperator & op, Schema * schema);
  ~Projection();
  
  virtual const Schema * schema() const;
  virtual bool moveNext();
  virtual void next(MemoryBlock &);
  virtual void reset() {}
  virtual void dump(std::ostream & strm,
		    char fs = '|', char rs = '\n');
};

#endif
