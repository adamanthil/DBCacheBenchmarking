#include "IRelationalOperator.h"

bool IRelationalOperator::isMatch(const Tuple & t)
{
  return m_boolExp != NULL ? 
    m_boolExp->evaluate(t) : false;
}

bool IRelationalOperator::isMatch(const Tuple & t1, 
				  const Tuple & t2)
{
  return m_boolExp != NULL ? 
    m_boolExp->evaluate(t1, t2) : false;
}
