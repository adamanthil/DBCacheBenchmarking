#ifndef PROJECTION_H_
#define PROJECTION_H_

#include <iostream>
#include <vector>

#include "IRelationalOperator.h"
#include "Schema.h"
#include "Tuple.h"
#include "Column.h"

typedef Columns ProjectionList;

class Projection : public IRelationalOperator
{
 private:
  enum { IN = 0, OUT = 1 };

  /** minimum space required to store tuple. */
  int m_rsize;
  /** flag to check if all data from current buffer has been consumed. */
  bool m_consumed; 
  /** next record id to consume. */
  int m_next; 

  MemoryBlock * m_buffer[2];
  IRelationalOperator * m_child;

  Tuple m_tuple;
  byte * m_data;
  Schema * m_schema;
 public:
  Projection(IRelationalOperator * op, ProjectionList & projList);
  ~Projection();
  
  virtual const Schema * schema() const;
  virtual bool moveNext();
  virtual void next(MemoryBlock &);
  virtual void reset() {}
  virtual void dump(std::ostream & strm,
		    char fs = '|', char rs = '\n');

  virtual void layout(const MaterializationLayout *) { }
};

#endif
