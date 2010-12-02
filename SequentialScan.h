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
  FileDescriptor * m_fd;
  MemoryBlock * m_buffer;

  byte * m_record;
  size_t m_recordSz;

  Schema * m_schema;
  std::vector<Attribute *> & m_attributes;
  
 public:
  SequentialScan(const std::string & filename,
		 std::vector<Attribute *> & attributes);

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
