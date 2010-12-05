#include "Schema.h"

Schema::Schema() :
  m_size(0)
{
}

void Schema::add(const Attribute * attribute)
{
  push_back(attribute);
  m_size += attribute->size();
}

int Schema::offset(const Attribute * attribute) const
{
  int offset = 0;
  for (int i = 0; i < size(); i++)
    {
      const Attribute * a = at(i);
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

const Attribute * Schema::operator[](const std::string & name) const
{
  const Attribute * attribute = NULL;

  for (int i = 0; i < size(); i++)
    {
      if (name == (attribute = at(i))->name())
	{
	  return attribute;
	}
    }

  return NULL;
}
