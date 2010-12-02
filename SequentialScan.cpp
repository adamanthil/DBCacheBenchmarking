#include <stdio.h>
#include <iostream>

#include "SequentialScan.h"
#include "FileManager.h"

#include "BufferManager.h"

SequentialScan::SequentialScan(const std::string & filename,
			       Schema * inSchema,
			       Schema * outSchema)
{
  m_fd = FileManager::getInstance()->open(filename);
  m_buffer = BufferManager::getInstance()->allocate();
  
  m_schema[IN] = inSchema;
  m_schema[OUT] = outSchema;

  for (int i = 0; i < 2; i++)
    {
      m_data[i] = new byte[m_schema[i]->rsize()];
    }
}

SequentialScan::~SequentialScan()
{
  if (m_buffer != NULL)
    {
      BufferManager::getInstance()->deallocate(m_buffer);
    }

  if (m_fd != NULL)
    {
      FileManager::getInstance()->close(m_fd);
    }
  
  for (int i = 0; i < 2; i++)
    {
      delete [] m_data[i];
    }
}

bool SequentialScan::moveNext() 
{
  int offset = 0;
  int available = m_buffer->capacity();
  size_t rsize = m_schema[OUT]->rsize();

  m_buffer->clear();
  int nrecords = 0;
  while (!m_fd->eof() && available >= rsize)
    {

      DiskPage * page = BufferManager::getInstance()->read(m_fd);
      
      for (int rid = 0; rid < page->size() && available >= rsize; rid++)
	{ 
	  
	  //if (satisfies(....)
	  /* create tuple */
	  for (int i = 0; i < m_schema[OUT]->nitems(); i++)
	    {
	      Attribute * attribute = (*m_schema[OUT])[i];
	      
	      memset(m_data[OUT], 0, rsize);
	      page->get(rid, attribute->name(), m_data[OUT], 
			attribute->size());
	      offset = m_buffer->put(m_data[OUT], offset, 
				     attribute->size());
	    }  
	  
	  available -= rsize;
	  nrecords++;
	
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
