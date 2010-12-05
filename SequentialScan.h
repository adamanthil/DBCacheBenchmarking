#ifndef SEQUENTIAL_SCAN_H_
#define SEQUENTIAL_SCAN_H_

#include <vector>
#include <string>

#include "FileDescriptor.h"
#include "IRelationalOperator.h"
#include "Attribute.h"
#include "MemoryBlock.h"
#include "Clause.h"

class SequentialScan : public IRelationalOperator
{
 private:
  FileDescriptor * m_fd;
  MemoryBlock * m_buffer;
  WhereClause * m_clause;
  const Schema * m_schema;
  byte * m_data;

  byte * extract(byte *, const DiskPage *, int, const Schema *);
  
 public:
  SequentialScan(const std::string & filename,
		 const Schema * schema);
  SequentialScan(const std::string & filename, const Schema * schema,
		 WhereClause * clause);
  ~SequentialScan();

  virtual const Schema * schema() const;
  virtual bool moveNext();
  virtual void next(MemoryBlock & buffer);
  virtual void reset();
};

#endif
