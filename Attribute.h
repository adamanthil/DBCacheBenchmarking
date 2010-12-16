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
  int m_id; 
  int m_position; // relative position in schema
  std::string m_table;
  std::string m_name;
  std::string m_qname;
  size_t m_size;
  field_type_t m_type;
public:
  Attribute(int id, int position, const std::string & name, 
	    const std::string & table, size_t size, field_type_t type);

  int id() const;
  int position() const;
  const std::string & qualified_name() const;
  const std::string & qualifiedName() const { return qualified_name(); }

  const std::string & table() const;
  const std::string & name() const;
  size_t size() const;
  field_type_t type() const;

  static field_type_t type(const std::string &);
  static const char * description(field_type_t type);
};

#endif
