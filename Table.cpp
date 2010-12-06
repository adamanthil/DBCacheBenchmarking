#include "Table.h"

Table::Table(int tid, std::string name, std::string path, Schema * schema)
  : m_id(tid), m_name(name), m_path(path)
{
  m_schema = schema;
}

Table::~Table()
{
  delete m_schema;
}

const Schema * Table::schema() const 
{
  return m_schema;
}

const std::string & Table::name() const 
{
  return m_name;
}

int Table::id() const
{
  return m_id;
}

const std::string & Table::path() const 
{
  return m_path;
}

