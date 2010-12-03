#include "Schema.h"

Schema::Schema() :
  m_size(0)
{
}

void Schema::add(Attribute * attribute)
{
  push_back(attribute);
  m_size += attribute->size();
}

int Schema::offset(const Attribute * attribute) const
{
  int offset = 0;
  for (int i = 0; i < size(); i++)
    {
      Attribute * a = at(i);
      if (a == attribute)
	{
	  return offset;
	}
      offset += a->size();
    }
  
  return -1;
}

size_t Schema::rsize() const
{
  return m_size;
}

size_t Schema::nitems() const
{
  return size();
}
