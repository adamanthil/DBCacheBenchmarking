#include "Tuple.h"

void Tuple::data(byte * values)
{
  m_data = values;
}

void Tuple::dump(std::ostream & output, 
		 char fs, char rs)
{
  int offset = 0;
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
	  char buffer[256 + 1];
	  memset(buffer, 0, sizeof(buffer));
	  memcpy(buffer, m_data + offset, attribute->size());
	  output << buffer;
	  break;
	}

      offset += attribute->size();

      output << (i < m_schema->nitems() - 1 ? fs : rs);
    }
}
