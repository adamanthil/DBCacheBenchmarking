#ifndef SEQUENTIAL_SCAN_H_
#define SEQUENTIAL_SCAN_H_

#include <vector>
#include <string>

#include "filedescriptor.h"
#include "irelationaloperator.h"
#include "attribute.h"

class SequentialScan : public IRelationalOperator
{
 private:
  FileDescriptor * m_fd;
  MemoryBuffer m_buffer;
  Schema m_schema;
  
 public:
  public SequentialScan(std::string filename, vector<Attribute> attributes);
  /*
  public SequentialScan(std::string filename, vector<Attribute> attributes,
			Predicate predicate);
  */
  virtual const Schema & schema();
  virtual boolean moveNext();
  virtual void next(MemoryBuffer buffer);
};

#endif
