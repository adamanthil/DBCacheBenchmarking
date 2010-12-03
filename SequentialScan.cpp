#include <new>
#include <stdio.h>
#include <iostream>

#include "SequentialScan.h"
#include "FileManager.h"
#include "BufferManager.h"

SequentialScan::SequentialScan(const std::string & filename, Schema * schema)
{
  m_fd = FileManager::getInstance()->open(filename);
  m_buffer = BufferManager::getInstance()->allocate();
  m_clause = NULL;
  m_schema = schema;
  m_data = new byte[m_schema->rsize()];
}

SequentialScan::SequentialScan(const std::string & filename, Schema * schema,
			       WhereClause * clause)
{
  m_fd = FileManager::getInstance()->open(filename);
  m_buffer = BufferManager::getInstance()->allocate();
  m_schema = schema;
  m_clause = clause;
  m_data = new byte[m_schema->rsize()];
}

SequentialScan::~SequentialScan()
{
  BufferManager::getInstance()->deallocate(m_buffer);
  FileManager::getInstance()->close(m_fd);
  delete [] m_data;
}

const Schema * SequentialScan::schema() const
{
  return m_schema;
}

byte * SequentialScan::extract(byte * buffer, const DiskPage * page, 
			       int rid, const Schema * schema)
{
  int offset = 0;
  for (int i = 0; i < schema->nitems(); i++)
    {
      const Attribute * attribute = schema->at(i);
      page->get(rid, attribute->name(), buffer + offset, attribute->size());
      offset += attribute->size();
    }
  
  return buffer;
}

bool SequentialScan::moveNext() 
{
  int nrecords = 0;
  int offset = 0;
  int available = m_buffer->capacity();
  size_t rsize = m_schema->rsize();

  Tuple tuple;
  byte buffer[256]; // TODO: 

  tuple.m_data = buffer;
  m_buffer->clear();
  while (!m_fd->eof() && available >= rsize)
    {

      DiskPage * page = BufferManager::getInstance()->read(m_fd);
      
      for (int rid = 0; rid < page->size() && available >= rsize; rid++)
	{ 

	  /* extract data to update variable expressions. */
	  if (m_clause != NULL)
	    {
	      extract(buffer, page, rid, m_clause->schema()); 
	    }

	  /* extract tuple */
	  if (m_clause == NULL || m_clause->evaluate(tuple))
	    {
	      offset = m_buffer->put(extract(m_data, page, rid, m_schema), 
				     offset, m_schema->rsize());
	      available -= rsize;
	      nrecords++;
	    }
	}
    }

  m_buffer->setSize(nrecords);
  return  nrecords > 0;
}

void SequentialScan::next(MemoryBlock & block)
{
  block.copy(*m_buffer);
}

void SequentialScan::reset()
{
  m_fd->reset();
}
