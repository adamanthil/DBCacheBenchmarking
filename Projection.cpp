#include "Projection.h"
#include "BufferManager.h"
#include "Tuple.h"

Projection::Projection(IRelationalOperator & child,
		       std::vector<Attribute *> & attributes) :
  m_child(child)
{
  m_buffer = BufferManager::getInstance()->allocate();
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
  int recsize = 0;
  while (m_child.moveNext())
    {
      int offset = 0;
      
      m_buffer->clear();
      m_child.next(*m_buffer);
      for (int i = 0; i < m_buffer->getSize(); i++)
	{
	  //	  tuple.data(m_buffer + offset);
	  //ostream << tuple.toString(fs) << rs;
	}
    }

  stream << std::endl;
}
