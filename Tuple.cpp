#include <new>
#include "Tuple.h"

Tuple::Tuple()
{
  m_data = NULL;
  m_schema = NULL;
}

void Tuple::schema(const Schema * schema)
{
  m_schema = schema;
}

void Tuple::data(byte * values)
{
  m_data = values;
}

void Tuple::value(int * buffer, const Attribute & attribute) const
{
  int offset = m_schema->offset(&attribute);
  memcpy(buffer, m_data + offset, sizeof(int));
}

void Tuple::dump(std::ostream & output, char fs, char rs)
{
  int offset = 0;
  char * buffer = new char[m_schema->rsize()];
  for (int i = 0; i < m_schema->nitems(); i++)
    {
      Attribute * attribute = m_schema->at(i);

      switch (attribute->type())
	{
	case BIT:
	case CHAR:
	  output << m_data[offset];
	  break;
	case INTEGER:
	  output << *(int *)(m_data + offset);
	  break;
	case STRING:
	  memset(buffer, 0, m_schema->rsize());
	  memcpy(buffer, m_data + offset, attribute->size());
	  output << buffer;
	  break;
	}

      offset += attribute->size();

      output << (i < m_schema->nitems() - 1 ? fs : rs);
    }

  delete [] buffer;
}
