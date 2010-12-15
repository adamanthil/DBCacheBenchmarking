#include <fstream>

#include "Database.h"
#include "Attribute.h"

Database * Database::db = NULL;

Database::Database(const std::string & filename)
{
  parse_file(filename);
}

Database::~Database()
{
}

void Database::Initialize(std::string filename)
{
  if (Database::db == NULL)
    {
      Database::db = new Database(filename);
    }
}

Database * Database::getInstance()
{
  if (!Database::db)
    {
      Database::Initialize();
    }

  return Database::db;
}

const char * Database::get_value(const std::string & xml, const char * element, 
				 char * value, int size)
{
  std::string e = std::string(element) + "=";
  std::string s = xml.substr(xml.find(e));
  s = s.substr(s.find('"') + 1);
  s = s.substr(0, s.find('"'));
	    
  strncpy(value, s.c_str(), size);
  return value;
}

void Database::parse_file(const std::string & filename)
{
  Schema * schema;

  char buffer[4096];
  std::string s;
  std::ifstream fp(filename.c_str());

  fp.getline(buffer, sizeof(buffer)); // read <database>
  fp.getline(buffer, sizeof(buffer)); // read <tables>

  fp.getline(buffer, sizeof(buffer));
			        
  /* parse tables */
  while (!fp.eof() && (s = buffer).find("<table") != std::string::npos)
  {
    schema = new Schema();
    char value[256];
    int tid = atoi(get_value(s, "id",value,sizeof(value)));    
    std::string tname = get_value(s, "name",value,sizeof(value));
    std::string path = get_value(s, "path",value,sizeof(value));

    /* parse schema */
    fp.getline(buffer, sizeof(buffer)); // read <schema>
    fp.getline(buffer, sizeof(buffer)); // read first attribute
    while ((s = buffer).find("</schema>") == std::string::npos)
    {
      int aid = atoi(get_value(s, "id",value,sizeof(value)));
      std::string name = get_value(s, "name",value,sizeof(value));
      std::string type = get_value(s, "type",value,sizeof(value));
      int length = atoi(get_value(s, "length",value,sizeof(value)));

      fp.getline(buffer, sizeof(buffer));

      Attribute a(aid, 0, name, tname, length, Attribute::type(type));
      schema->add(&a);
    } 
    m_tables.push_back(new Table(tid, tname, path, schema));

    fp.getline(buffer, sizeof(buffer)); // read </table>
    fp.getline(buffer, sizeof(buffer)); // read 
  }

  fp.close();
}

const Table * Database::table(const std::string & table) const
{
  for (int i = 0; i < m_tables.size(); i++)
    {
      if (m_tables[i]->name() == table)
	return m_tables[i];
    }

  return NULL;
}

const Tables & Database::tables() const
{
  return m_tables;
}
