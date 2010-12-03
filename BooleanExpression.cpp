#include "BooleanExpression.h"

BooleanExpression::BooleanExpression(unsigned int ndisjuncts) :
  m_ndisjuncts(ndisjuncts)
{
  m_clauses = new BooleanTerm *[m_ndisjuncts];
  for (int i = 0; i < m_ndisjuncts; i++)
    m_clauses[i] = NULL;
}

BooleanExpression::~BooleanExpression()
{
  delete [] m_clauses;
}

void BooleanExpression::term(unsigned int disjunct, BooleanTerm * term)
{
  m_clauses[disjunct] = term;
}

bool BooleanExpression::evaluate()
{
   bool flag = false;

   for (int i = 0; i < m_ndisjuncts && !flag; i++)
     {
       if (m_clauses[i])
	 flag = m_clauses[i]->value();
     }
   
  return flag;
}

void BooleanTerm::factor(IBooleanFactor * f)
{
  m_factors.push_back(f);
}

size_t BooleanTerm::nfactors() const
{
  return m_factors.size();
}

bool BooleanTerm::value()
{
  bool flag = true;
  int n = m_factors.size();

  for (int i = 0; i < n && flag; i++)
    {
      flag &= m_factors[i]->value();
    }

  return flag;
}
