#include "Operand.h"

template<> int VariableOperand<int>::value()
{
  m_tuple->value((int *)m_buffer, *m_attribute);
  return *(int *)m_buffer;
}

template<> const char * VariableOperand<const char *>::value()
{
  memset(m_buffer, 0, m_attribute->size());
  m_tuple->value((char *)m_buffer, *m_attribute);
  return (const char *)m_buffer;
}


