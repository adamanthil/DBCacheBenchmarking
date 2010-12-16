#include <algorithm>
#include "Column.h"

Columns::~Columns()
{
  std::for_each(begin(), end(), free);
}

void Columns::add(const std::string & name, const std::string table)
{
  Column * c = new Column();
  c->m_name = name;
  c->m_table = table;
  c->m_qualified_name = table + "." + name;
  push_back(c); 
}

bool Columns::contains(const std::string & column) const
{
  return false; // TODO: 
}

const Column * Columns::at(int c) const
{
  return std::vector<Column *>::at(c);
}

const Column * Columns::operator[](int c) const
{
  return at(c);
}

int Columns::count() const
{
  return size();
}
