#include <new>

#include "Projection.h"
#include "BufferManager.h"
#include "Tuple.h"

Projection::Projection(IRelationalOperator & child,
		       Schema & schema) :
  m_child(child)
{
  m_rsize = schema.rsize();
  m_buffer = BufferManager::getInstance()->allocate();
  m_tuple.schema(&schema);
}

bool Projection::moveNext()
{
  return m_child.moveNext();
}

void Projection::next(MemoryBlock & buffer)
{
  m_buffer->clear();
  m_child.next(*m_buffer);
  buffer.copy(*m_buffer);
}

void Projection::dump(std::ostream & stream, char fs, char rs)
{
  byte * data = new byte[m_rsize];

  m_tuple.data(data);

  while (m_child.moveNext())
    {
      int offset = 0;
      
      m_buffer->clear();
      m_child.next(*m_buffer);
      for (int i = 0; i < m_buffer->getSize(); i++)
	{
	  m_buffer->get(data, i * m_rsize, m_rsize);
	  m_tuple.dump(stream, fs, rs);
	}
    }

  stream << std::endl;
  delete [] data;
}
