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

size_t Schema::rsize() const
{
  return m_size;
}

size_t Schema::nitems() const
{
  return size();
}
