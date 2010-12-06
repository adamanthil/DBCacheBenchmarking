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

void Query::profile()
{
  MemoryBlock * buffer = BufferManager::getInstance()->allocate();
  while (m_op->moveNext())
    {
      m_op->next(*buffer);
      m_nrecords += buffer->getSize();
      m_nfetches++;
    }
  BufferManager::getInstance()->deallocate(buffer);
}

void Query::stats(std::ostream & strm)
{
  strm << m_id << ": " << m_nrecords << ", " << m_nfetches << ", "
       << m_stime << ", " << m_etime << ", " << m_duration << std::endl;
}

void * Query::execute()
{
  return NULL;
}
