#include <new>
#include "Predicate.h"

BooleanExpression::BooleanExpression(int ndisjuncts) :
  m_ndisjuncts(ndisjuncts)
{
  m_predicates = new std::vector<IPredicate *>[m_ndisjuncts];
  for (int i = 0; i < m_ndisjuncts; i++)
    {
      //      m_predicates[i] = new std::vector<IPredicate *>();
    }
}

BooleanExpression::~BooleanExpression()
{
  for (int i = 0; i < m_ndisjuncts; i++)
    {
      //      delete m_predicates[i];
    }
  delete [] m_predicates;
}

void BooleanExpression::factor(int disjunct, IPredicate * p)
{
  // TODO: determine if varible included in factor.
  Predicate<void> * pp = (Predicate<void> *)p;

  if (((Operand<void> *)pp->m_lOperand)->type() == VARIABLE)
    {
      m_variables.push_back((IVariableOperand *)pp->m_lOperand);
    }

  if (((Operand<void> *)pp->m_rOperand)->type() == VARIABLE)
    {
      m_variables.push_back((IVariableOperand *)pp->m_rOperand);
    }
  m_predicates[disjunct].push_back(p);
}

const std::vector<IVariableOperand *> * BooleanExpression::variables()
{
  return &m_variables;
}

bool BooleanExpression::evaluate()
{
  bool flag = false;

  // or'd-predicates.
  for (int i = 0; i < m_ndisjuncts && !flag; i++)
    {
      int n = m_predicates[i].size();
      // and-predicates
      flag = true;
      for (int j = 0; j < n && flag; j++)
	{
	  flag |= m_predicates[i][j]->eval();
	}
    }
  
  return flag;
}
