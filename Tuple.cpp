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

const Schema * Tuple::schema() const 
{
  return m_schema;
}

void Tuple::value(int * buffer, const Attribute & attribute) const
{
  value(buffer, sizeof(int), attribute);
}

void Tuple::value(char * buffer, const Attribute & attribute) const
{
  value(buffer, attribute.size(), attribute);
}

void Tuple::value(void * buffer, size_t length, 
		  const Attribute & attribute) const
{
  int offset = m_schema->offset(&attribute);
  memcpy(buffer, m_data + offset, length);
}

void Tuple::map(const Tuple * other)
{
  int offset = 0;

  memset(m_data, 0, m_schema->rsize());
  for (int i = 0; i < m_schema->nitems(); i++)
    {
      const Attribute * attribute = m_schema->at(i);
      other->value(m_data + offset, attribute->size(), *attribute);
      offset += attribute->size();
    }
}

void Tuple::dump(std::ostream & output, char fs, char rs)
{
  int offset = 0;
  char * buffer = new char[m_schema->rsize() + 1]; // + 1 for eof marker
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
