#ifndef ATTRIBUTE_H_
#define ATTRIBUTE_H_

#include <string>

typedef enum 
{
  BIT = 0,
  INTEGER,
  REAL,
  CHAR,
  STRING,
} field_type_t;

class Attribute 
{
private:
  std::string m_table;
  std::string m_name;
  size_t m_size;
  field_type_t m_type;
public:
  Attribute(std::string name, std::string table, 
	    size_t size, field_type_t type);
  std::string qualifiedName() const;
  std::string table() const;
  std::string name() const;
  size_t size() const;
  field_type_t type() const;
};

#endif
