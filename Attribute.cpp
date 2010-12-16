#include "Attribute.h"

Attribute::Attribute(int id, int position, const std::string & name, 
		     const std::string & table, size_t size, field_type_t type) :
  m_id(id), m_position(position), m_name(name), m_table(table), m_qname(table + "." + name),
  m_size(size), m_type(type)
{
}

int Attribute::id() const
{
  return m_id;
}

int Attribute::position() const
{
  return m_position;
}

const std::string & Attribute::name() const 
{
  return m_name;
}

const std::string & Attribute::table() const
{
  return m_table;
}

const std::string & Attribute::qualified_name() const 
{
  return m_qname;
}

size_t Attribute::size() const 
{
  return m_size;
}

field_type_t Attribute::type() const
{
  return m_type;
}

field_type_t Attribute::type(const std::string & t)
{
  switch (t[0])
    {
    case 'I':
      return INTEGER;
    case 'R':
      return REAL;
    case 'C':
      return CHAR;
    case 'S':
      return STRING;
    case 'B':
    default:
      return BIT;
  }
}

const char * Attribute::description(field_type_t t)
{
  switch (t)
    {
    case INTEGER:
      return "INTEGER";
    case REAL:
      return "REAL";
    case CHAR:
      return "CHAR";
    case STRING:
      return "STRING";
    case BIT:
    default:
      return "BIT";
    }
}
