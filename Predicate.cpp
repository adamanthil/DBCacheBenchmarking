#include "Predicate.h"

template<typename T, typename K>
bool Predicate<T,K>::eval(const Tuple & t)
{
  switch (m_op)
    {
    case EQ:
      return t.value(m_lOperand.value) == m_rOperand.value;
    case NE:
      return t.value(m_lOperand.value) != m_rOperand.value;
    case LT:
      return t.value(m_lOperand.value) < m_rOperand.value;
    case LE:
      return t.value(m_lOperand.value) <= m_rOperand.value;
    case GT:
      return t.value(m_lOperand.value) > m_rOperand.value;
    case GE:
      return t.value(m_lOperand.value) >= m_rOperand.value;
    default:
      return false;
    }
}

bool BooleanExpression::evaluate(const Tuple & t)
{
  bool flag = false;
  int m = 0;

  // or'd-predicates.
  for (int i = 0; i < m && !flag; i++)
    {
      int n = 0;
      // and-predicates
      flag = true;
      for (int j = 0; j < n && flag; j++)
	{
	  flag |= m_predicates[i][j].eval(t);
	}
    }
  
  return flag;
}

bool BooleanExpression::evaluate(const Tuple & t1, const Tuple & t2)
{
  return false;
}
