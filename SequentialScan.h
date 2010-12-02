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
  Tuple * m_tuple[2];
  BooleanExpression * whereClause;
  
 public:
  SequentialScan(const std::string & filename,
		 Schema * inSchema, Schema * outSchema);
  SequentialScan(const std::string & filename,
		 Schema & inSchema, Schema & outSchema,
		 BooleanExpression & whereClause);

  /*
  public SequentialScan(std::string & filename, 
			std::vector<Attribute> & attributes,
			Predicate predicate);
  
  public SequentialScan(std::string filename, vector<Attribute> & attributes,
			vector<Predicate> & predicates);
  public SequentialScan(std::string filename, vector<Attribute> & attributes,
			vector<<vector<Predicate>> & predicates);
  */

  ~SequentialScan();

  //  virtual const Schema & schema();
  virtual bool moveNext();
  virtual void next(MemoryBlock & buffer);
  virtual void reset();
};

#endif
