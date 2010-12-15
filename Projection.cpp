#include <new>

#include "Projection.h"
#include "BufferManager.h"
#include "Tuple.h"

Projection::Projection(IRelationalOperator * child, ProjectionList & columns) 
  : m_consumed(true), m_next(0), m_child(child)
{

  m_schema = new Schema();
  for (int i = 0; i < columns.count(); i++)
    {
      const Column * column = columns[i];
      std::string c = column->m_table + "." + column->m_name;
      m_schema->add((*m_child->schema())[c]);
    }
  m_rsize = m_schema->rsize();

  m_tuple.schema(m_child->schema());
  m_tuple.m_data = new byte[m_child->schema()->rsize()];
  m_data = new byte[m_rsize];

  for (int i = 0; i < sizeof(m_buffer) / sizeof(m_buffer[0]); i++)
    m_buffer[i] = BufferManager::getInstance()->allocate();
}

Projection::~Projection()
{
  for (int i = 0; i < sizeof(m_buffer) / sizeof(m_buffer[0]); i++)
    BufferManager::getInstance()->deallocate(m_buffer[i]);

  delete m_child;
  delete m_schema;
  delete [] m_tuple.m_data;
  delete [] m_data;
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
	  if (m_child->moveNext())
	    m_child->next(*m_buffer[IN]);
	  else
	    break; // terminate loop.
	}

      for (; m_next < m_buffer[IN]->getSize() && m_rsize <= available; m_next++)
	{
	  // retrieve tuple from memory buffer.
	  m_buffer[IN]->get(m_tuple.m_data, 
			    m_next * m_tuple.schema()->rsize(), 
			    m_tuple.schema()->rsize());

	  // extract projected data.
	  int to = 0;
	  for (int i = 0; i < m_schema->nitems(); i++)
	    {
	      const Attribute * attribute = m_schema->at(i);
	      m_tuple.value((char *)m_data + to, *attribute);
	      to += attribute->size();
	    }

	  // copy extracted data to memory buffer.
	  m_buffer[OUT]->put(m_data, offset, m_rsize);

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
  return m_schema;
}

void Projection::dump(std::ostream & stream, char fs, char rs)
{

  Tuple t;
  t.schema(m_schema);
  t.m_data = m_data;
  
  while (moveNext())
    { 
      for (int i = 0; i < m_buffer[OUT]->getSize(); i++)
	{
	  m_buffer[OUT]->get(m_data, i * m_rsize, m_rsize);
	  t.dump(stream, fs, rs);
	}
    }

  stream << std::endl;
}
