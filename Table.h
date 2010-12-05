#ifndef TABLE_H_
#define TABLE_H_

#include "Schema.h"
class Table
{
 private:
  int m_id;
  std::string m_name;
  std::string m_path;
  Schema * m_schema;
 public:
  Table(int id, std::string name, std::string path, Schema * schema);
  const Schema * schema() const;
  const std::string & name() const;
  const std::string & path() const;
};

#endif
