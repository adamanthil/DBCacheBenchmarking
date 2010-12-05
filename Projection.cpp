#include <new>

#include "Projection.h"
#include "BufferManager.h"
#include "Tuple.h"

Projection::Projection(IRelationalOperator & child, const Schema * schema) :
  m_child(child), m_consumed(true), m_next(0)
{
  m_rsize = schema->rsize();
 
  for (int i = 0; i < sizeof(m_buffer) / sizeof(m_buffer[0]); i++)
    m_buffer[i] = BufferManager::getInstance()->allocate();

  m_tuple[IN].schema(m_child.schema());
  m_tuple[OUT].schema(schema);

  for (int i = 0; i < sizeof(m_tuple) / sizeof(m_tuple[0]); i++) 
      m_tuple[i].m_data = new byte[m_tuple[i].schema()->rsize()];
}

Projection::~Projection()
{
  for (int i = 0; i < sizeof(m_buffer) / sizeof(m_buffer[0]); i++)
    BufferManager::getInstance()->deallocate(m_buffer[i]);

  for (int i = 0; i < sizeof(m_tuple) / sizeof(m_tuple[0]); i++)
    delete [] m_tuple[i].m_data;
}

bool Projection::moveNext()
{
  int offset = 0;
  int nrecords = 0;
  int available = m_buffer[OUT]->capacity();

  while (available >= m_rsize)
    {
      // determine if we consumed all data from previous pass. 
      if (m_consumed)
	{
	  m_next = 0;
	  m_consumed = false;

	  m_buffer[IN]->clear();
	  if (m_child.moveNext())
	    m_child.next(*m_buffer[IN]);
	  else
	    break; // terminate loop.
	}

      for (; m_next < m_buffer[IN]->getSize() && m_rsize <= available; m_next++)
	{
	  // retrieve tuple from memory buffer.
	  m_buffer[IN]->get(m_tuple[IN].m_data, 
			    m_next * m_tuple[IN].schema()->rsize(), 
			    m_tuple[IN].schema()->rsize());
	  // extract required data.
	  m_tuple[OUT].map(&m_tuple[IN]);
	  // copy tuple to memory buffer.
	  m_buffer[OUT]->put(m_tuple[OUT].m_data, offset, m_rsize);

	  // book-keeping.
	  offset += m_rsize;
	  available -= m_rsize;
	  nrecords++;
	}

      m_consumed = m_next >= m_buffer[IN]->getSize();
    } 
  
  m_buffer[OUT]->setSize(nrecords);
  return nrecords > 0;
}

void Projection::next(MemoryBlock & buffer)
{
  buffer.copy(*m_buffer[OUT]);
}

const Schema * Projection::schema() const
{
  return m_tuple[OUT].schema();
}

void Projection::dump(std::ostream & stream, char fs, char rs)
{
  
  while (moveNext())
    { 
      
      for (int i = 0; i < m_buffer[OUT]->getSize(); i++)
	{
	  m_buffer[OUT]->get(m_tuple[OUT].m_data, i * m_rsize, m_rsize);
	  m_tuple[OUT].dump(stream, fs, rs);
	}
    }

  stream << std::endl;
}
