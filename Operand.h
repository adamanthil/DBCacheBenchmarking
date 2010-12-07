#ifndef OPERAND_H_
#define OPERAND_H_

#include "Attribute.h"
#include "Tuple.h"
#include "globals.h"

enum Type { VARIABLE, CONSTANT };

template<typename T>
class IOperand
{
 public: 
  virtual T value() = 0;
  virtual int compareTo(IOperand<T> & other) = 0;
};

template<typename T>
class Operand : public IOperand<T>
{
 protected:
  Type m_type;
  field_type_t m_fieldType;
 public:
  Operand(Type type, field_type_t fldType);
  virtual T value() = 0;
  virtual int compareTo(IOperand<T> & other);
  virtual Type type() { return m_type; }
};

template<typename T>
class ConstantOperand : public Operand<T>
{
 private:
  T m_value;
 public:
  ConstantOperand(T value, field_type_t type);
  virtual T value();
};

class IVariableOperand
{
 public: 
  virtual void data(const Tuple * t) = 0;
};

template<typename T>
class VariableOperand : public Operand<T>, public IVariableOperand
{
 private:
  const std::string & m_name;
  size_t m_size;
  byte * m_buffer; // value buffer

  const Tuple * m_tuple; // data buffer
 public:
  VariableOperand(const std::string & name, field_type_t type, size_t size);
  ~VariableOperand();
  virtual T value();
  virtual void data(const Tuple * t);
};

template<typename T>
Operand<T>::Operand(Type opType, field_type_t fieldType)
  : m_type(opType), m_fieldType(fieldType)
{
}

template<typename T>
int Operand<T>::compareTo(IOperand<T> & other)
{
  switch (m_fieldType)
    {
    case INTEGER:
    case BIT:
    case CHAR:
      return value() - other.value();
    case STRING:
      {
      char * v1 = (char *)value();
      char * v2 = (char *)other.value();
      return strcmp((char *)value(), (char *)other.value());
      }
    case REAL:
      return 0;
    }
}

template<typename T>
ConstantOperand<T>::ConstantOperand(T value, field_type_t type)
: Operand<T>(CONSTANT, type), m_value(value)
{
}

template<typename T>
T ConstantOperand<T>::value()
{
  return m_value;
}

template<typename T>
VariableOperand<T>::VariableOperand(const std::string & name, field_type_t type,
				    size_t size) :
Operand<T>(VARIABLE, type), m_size(size), m_name(name), m_tuple(NULL)
{
  m_buffer = new byte[size];
  memset(m_buffer, 0, size);
}

template<typename T>
VariableOperand<T>::~VariableOperand()
{
  delete [] m_buffer;
}

/*
template<typename T>
T VariableOperand<T>::value()
{
  return 0;
} 
*/

template<typename T>
void VariableOperand<T>::data(const Tuple * t)
{
  m_tuple = t;
}

#endif
