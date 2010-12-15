#ifndef DATABASE_H_
#define DATABASE_H_

#include <vector>

#include "Table.h"

typedef std::vector<Table *> Tables;

class Database
{
 private:
  Tables m_tables;
  static Database * db;
 public:
  ~Database();
  const Table * table(const std::string & name) const;
  const Table * table(int tid) const;
  const void table(Table * table);

  const Tables & tables() const;

  static void Initialize(std::string filename = "db.xml");
  static Database * getInstance();
 private:
  Database(const std::string & filename);
  void parse_file(const std::string & filename);
  const char * get_value(const std::string &, const char *,
			 char * buffer, int size);
  
};
#endif
