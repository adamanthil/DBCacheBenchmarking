#include <new>
#include <stdio.h>
#include <iostream>

#include "SequentialScan.h"
#include "FileManager.h"
#include "BufferManager.h"

SequentialScan::SequentialScan(const Table * table, const std::string & alias, 
			       const Columns & columns)
  : m_clause(NULL), m_rid(0), m_page(NULL)
{
  m_fd = FileManager::getInstance()->open(table->path());
  m_buffer = BufferManager::getInstance()->allocate();

  m_schema = new Schema();
  for (int i = 0; i < columns.count(); i++)
    {
      const Column * c = columns[i];
      const Attribute * a = (*table->schema())[c->m_name];
      Attribute attribute(a->id(), 0, a->name(), 
			  alias, a->size(), a->type());
      m_schema->add(&attribute);
    }
  m_data = new byte[m_schema->rsize()];

  m_tuple.m_data = NULL;
  m_tuple.schema(NULL);
}

SequentialScan::~SequentialScan()
{
  delete [] m_data;
  delete m_schema;
  delete m_clause;

  delete m_tuple.m_data;
  delete m_tuple.schema();

  BufferManager::getInstance()->deallocate(m_buffer);
  FileManager::getInstance()->close(m_fd);
}

void SequentialScan::filter(BooleanExpression * expression, const std::vector<std::string> & filterColumns)
{
  m_clause = new WhereClause(expression);

  Schema * schema = new Schema();
  for (int i = 0; i < filterColumns.size(); i++)
    {
      std::string s = filterColumns[i];
      const Attribute * a = (*m_schema)[s];

      schema->add(a);
    }

  m_tuple.m_data = new byte[schema->rsize()];
  m_tuple.schema(schema);
}

const Schema * SequentialScan::schema() const
{
  return m_schema;
}

bool SequentialScan::moveNext() 
{
  int nrecords = 0;
  int offset = 0;
  int available = m_buffer->capacity();
  size_t rsize = m_schema->rsize();

  m_buffer->clear();

  while (available >= rsize)
    {
      // determine if we consumed all the data on current page
      if (m_page == NULL || m_rid >= m_page->size())
	{
	  m_page = !m_fd->eof() ? BufferManager::getInstance()->read(m_fd) : NULL;
	  m_rid = 0;
	  
	  if (m_page == NULL)
	      break;
	}
      
      for (; m_rid < m_page->size() && available >= rsize; m_rid++)
	{ 
	  /* extract data to update variable expressions. */
	  if (m_clause != NULL)
	    {
	      m_page->get(m_rid, m_tuple.schema(), m_tuple.m_data, m_tuple.schema()->rsize());
	    }
	  
	  if (m_clause == NULL || m_clause->evaluate(m_tuple))
	    {
	      memset(m_data, 0, rsize);
	      m_page->get(m_rid, m_schema, m_data, rsize); 
	      offset = m_buffer->put(m_data, offset, rsize); // update to use smart buffer. 
	      available -= rsize;
	      nrecords++;
	    }
	}
    }

  m_buffer->setSize(nrecords);
  return nrecords > 0;
}

void SequentialScan::next(MemoryBlock & block)
{
  block.copy(*m_buffer);
}

void SequentialScan::reset()
{
  m_fd->reset();
}
