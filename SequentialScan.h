#ifndef SEQUENTIAL_SCAN_H_
#define SEQUENTIAL_SCAN_H_

#include <vector>
#include <string>

#include "Table.h"
#include "FileDescriptor.h"
#include "IRelationalOperator.h"
#include "Attribute.h"
#include "Column.h"
#include "MemoryBlock.h"
#include "Clause.h"

class SequentialScan : public IRelationalOperator
{
 private:
  FileDescriptor * m_fd;
  DiskPage * m_page;
  int m_rid;

  byte * m_data;
  MemoryBlock * m_buffer;
  Schema * m_schema;
  WhereClause * m_clause;

  Tuple m_tuple; // filtered tuple. 
  
 public:

  SequentialScan(const Table * relation, const std::string & alias,
		 const Columns &);
  ~SequentialScan();

  void filter(BooleanExpression * expression, const std::vector<std::string> & filterColumns);

  virtual const Schema * schema() const;
  virtual bool moveNext();
  virtual void next(MemoryBlock & buffer);
  virtual void reset();
};

#endif
