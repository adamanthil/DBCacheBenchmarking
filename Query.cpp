#include "Query.h"
#include "BufferManager.h"

Query::Query(int id, IRelationalOperator * op) :
  m_id(id), m_nrecords(0), m_nfetches(0), m_stime(0), m_etime(0), m_duration(0)
{
  m_op = op;
}

Query::~Query()
{
  delete m_op;
}

void Query::profile(int count)
{
  MemoryBlock * buffer = BufferManager::getInstance()->allocate();
  m_count = count;
  m_stime = clock();
  for (int i = 0; i < m_count; i++)
    {
      m_op->reset();
      while (m_op->moveNext())
	{
	  m_op->next(*buffer);
	  m_nrecords += buffer->getSize();
	  m_nfetches++;
	}
    }
  m_etime = clock();
  BufferManager::getInstance()->deallocate(buffer);
}

void Query::stats(std::ostream & strm)
{
  m_duration = (float)m_etime - (float)m_stime;
  strm << m_id << ": " << m_nrecords << ", " << m_nfetches << ", "
       << "time:[" << m_stime << ", " << m_etime << "], " 
       << m_duration << std::endl;
}

void * Query::execute()
{
  return NULL;
}
