#include <stdio.h>
#include <iostream>

#include "SequentialScan.h"
#include "FileManager.h"

#include "BufferManager.h"

SequentialScan::SequentialScan(const std::string & filename,
			       std::vector<Attribute *> & attributes) :
  m_attributes(attributes)
{
  
  m_fd = FileManager::getInstance()->open(filename);
  m_buffer = BufferManager::getInstance()->allocate();
  
  m_recordSz = 0;
  for (int i = 0; i < m_attributes.size(); i++)
    {
      m_recordSz += m_attributes[i]->size();
    }

  m_record = new byte[m_recordSz];
  m_schema = NULL;
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

  delete [] m_record;
}

bool SequentialScan::moveNext()
{
  int offset = 0;
  int available = m_buffer->capacity();

  m_buffer->clear();

  while (!m_fd->eof() && available >= m_recordSz)
    {
      std::cerr << "reading page" << std::endl;
      DiskPage * page =  BufferManager::getInstance()->read(m_fd);

      for (int rid = 0; rid < page->size() && available >= m_recordSz; rid++)
	{
	  std::cerr << "reading records on page" << std::endl;
	  
	  //if (isMatch(m_tuple.setValues(

	  /* create tuple */
	  for (int i = 0; i < m_attributes.size(); i++)
	    {
	      std::cerr << "getting attribute[" 
			<< "]" << std::endl;
	      memset(m_record, 0, m_recordSz);
	      page->get(rid, m_attributes[i]->name(), m_record, 
			m_attributes[i]->size());
	      std::cerr << m_record << std::endl;
	      offset = m_buffer->put(m_record, offset, m_attributes[i]->size());
	    }  
	}
    }
}

void SequentialScan::next(MemoryBlock & block)
{
  block.copy(*m_buffer);
}

void SequentialScan::reset()
{
  m_fd->reset();
}
