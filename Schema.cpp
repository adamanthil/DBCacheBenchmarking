#include <algorithm>

#include "Schema.h"

Schema::Schema() :
  m_size(0)
{
}

Schema::~Schema()
{
  std::for_each(begin(), end(), free);
}

void Schema::add(const Attribute * a)
{
  Attribute * attribute = 
    new Attribute(a->id(), size(), a->name(), a->table(),
		  a->size(), a->type());

  push_back(attribute);
  m_offset.push_back(m_size);
  m_size += attribute->size();
}

int Schema::offset(const Attribute * attribute) const
{
  return m_offset[attribute->position()];

  /*
  for (int i = 0; i < size(); i++)
    {
      const Attribute * a = at(i);
      if (a->id() == attribute->id())
	{
	}
    }
  
  return -1;
  */
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

const Attribute * Schema::operator[](int fid) const
{
  return std::vector<Attribute *>::at(fid);
}

const Attribute * Schema::at(int fid) const
{
  return std::vector<Attribute *>::at(fid);
}
