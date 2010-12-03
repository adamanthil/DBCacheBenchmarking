#include <new>
#include "Predicate.h"

BooleanExpression::BooleanExpression(int ndisjuncts) :
  m_ndisjuncts(ndisjuncts)
{
  m_predicates = new std::vector<IPredicate *>[m_ndisjuncts];
}

BooleanExpression::~BooleanExpression()
{
  delete [] m_predicates;
}

void BooleanExpression::factor(int disjunct, IPredicate * p)
{
  // TODO: determine if varible included in factor.
  Predicate<int> * pp = (Predicate<int> *)p;

  /*
  if (((Operand<void>)pp->m_lOperand).type() == VARIABLE)
    {
      
      m_variables.push_back((IVariableOperand *)&pp->m_lOperand);
      std::cerr << "pushing " << (IVariableOperand *)&pp->m_lOperand 
		<< "raw " << pp->m_lOperand
		<< std::endl;
    }

  if (((Operand<void> *)pp->m_rOperand)->type() == VARIABLE)
    {
      m_variables.push_back((IVariableOperand *)pp->m_rOperand);
      */ 
  m_predicates[disjunct].push_back(p);
}

std::vector<IVariableOperand *> * BooleanExpression::variables()
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
