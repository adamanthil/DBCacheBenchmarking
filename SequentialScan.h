#ifndef SEQUENTIAL_SCAN_H_
#define SEQUENTIAL_SCAN_H_

#include <vector>
#include <string>

#include "FileDescriptor.h"
#include "IRelationalOperator.h"
#include "Attribute.h"
#include "Predicate.h"
#include "MemoryBlock.h"

class SequentialScan : public IRelationalOperator
{
 private:
  enum { IN=0, OUT=1 };
  FileDescriptor * m_fd;
  MemoryBlock * m_buffer;

  Schema * m_schema[2];
  byte * m_data[2];
  Tuple m_tuple;
  BooleanExpression * whereClause;
  
 public:
  SequentialScan(const std::string & filename,
		 Schema * schema);
  SequentialScan(const std::string & filename, Schema * outSchema,
		 Schema * inSchema, BooleanExpression * whereClause);
  ~SequentialScan();

  //  virtual const Schema & schema();
  virtual bool moveNext();
  virtual void next(MemoryBlock & buffer);
  virtual void reset();
};

#endif
