#include "attribute.h"

Attribute::Attribute(std::string name, std::string table, 
		     size_t size, field_type_t type) :
  m_name(name), m_table(table), m_size(size), m_type(type)
{
}

std::string Attribute::name() const 
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
