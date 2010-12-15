#include "Operand.h"

template<> int VariableOperand<int>::value()
{
  m_tuple->value((int *)m_buffer, m_name);
  return *(int *)m_buffer;
}

template<> char VariableOperand<char>::value()
{
  m_tuple->value((char *)m_buffer, m_name);
  return *(char *)m_buffer;
}

template<> const char * VariableOperand<const char *>::value()
{
  memset(m_buffer, 0, m_size);
  m_tuple->value((char *)m_buffer, m_name);
  return (const char *)m_buffer;
}


