#include "sequentialscan.h"

SequentialScan::SequentialScan(std::string filename)
{
  m_fd = FileManager::instance().open(filename);
}

bool SequentialScan::moveNext()
{
  int offset = 0;
  int recsize = 0; //TODO: determine record size;
  int available = m_buffer.capacity();

  m_buffer.reset();
  while (!m_fd.eof() && remaining > recsize)
    {
      DiskPage * page = m_fd.read();
      for (int rid = 0; rid < page->size() && available >= recsize; rid++)
	{
	  // TODO: check predicate to determine if record should be added.
	  for (int i = 0; i < m_attributes.size(); i++)
	    {
	      Attribute attribute = m_attributes[i];
	      byte buffer[]; // TODO: 
	      page->get(rid, attribute.name(), buffer, ...);

	      offset = m_buffer.put(offset, buffer, ...);
	    }
	}
    }
}

void SequentialScan::next(MemoryBuffer & buffer)
{
  buffer.put(0, m_buffer.get(), m_buffer.size());
}
