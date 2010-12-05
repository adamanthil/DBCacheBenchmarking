#include "Attribute.h"

Attribute::Attribute(int id, std::string name, std::string table, 
		     size_t size, field_type_t type) :
  m_id(id), m_name(name), m_table(table), m_size(size), m_type(type)
{
}

field_type_t Attribute::type(const std::string & t)
{
  if (t == "BIT")
  {
    return BIT;
  }
  else if (t == "INTEGER")
  {
    return INTEGER;
  }
  else if (t == "REAL")
  {
    return REAL;
  }
  else if (t == "CHAR")
  {
    return CHAR;
  }
  else
  {
    return STRING;
  }
}

int Attribute::id() const
{
  return m_id;
}

const std::string & Attribute::name() const 
{
  return m_name;
}

std::string Attribute::table() const
{
  return m_table;
}

std::string Attribute::qualifiedName() const 
{
  return m_table + "." + m_name;
}

size_t Attribute::size() const 
{
  return m_size;
}

field_type_t Attribute::type() const
{
  return m_type;
}
