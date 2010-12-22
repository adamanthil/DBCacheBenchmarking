#include <algorithm>
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
  for (int i = 0; i < m_ndisjuncts; i++)
    delete m_clauses[i];

  delete [] m_clauses;
}

// TODO: can this be cleaned up??
void BooleanExpression::term(unsigned int disjunct, BooleanTerm * term)
{
  m_clauses[disjunct] = term;

  for (int i = 0; i < term->nfactors(); i++)
    {
      if (dynamic_cast<const BooleanFactor<int> *>(term->m_factors[i]))  
	{
	  const BooleanFactor<int> * f = (const BooleanFactor<int> *)term->m_factors[i]; 

	  if (f->m_lOperand->type() == VARIABLE)
	    m_variables.push_back(dynamic_cast<IVariableOperand *>(f->m_lOperand));
      
	  if (f->m_rOperand->type() == VARIABLE)
	    m_variables.push_back(dynamic_cast<IVariableOperand *>(f->m_rOperand));
	}
      else if (dynamic_cast<const BooleanFactor<char> *>(term->m_factors[i]))  
	{
	  const BooleanFactor<char> * f = (const BooleanFactor<char> *)term->m_factors[i]; 

	  if (f->m_lOperand->type() == VARIABLE)
	    m_variables.push_back(dynamic_cast<IVariableOperand *>(f->m_lOperand));
      
	  if (f->m_rOperand->type() == VARIABLE)
	    m_variables.push_back(dynamic_cast<IVariableOperand *>(f->m_rOperand));
	}
    
      else if (dynamic_cast<const BooleanFactor<const char *> *>(term->m_factors[i]))  
	{
	  const BooleanFactor<const char *> * f = (const BooleanFactor<const char *> *)term->m_factors[i]; 
	  
	  if (f->m_lOperand->type() == VARIABLE)
	    m_variables.push_back(dynamic_cast<IVariableOperand *>(f->m_lOperand));
	  
	  if (f->m_rOperand->type() == VARIABLE)
	    m_variables.push_back(dynamic_cast<IVariableOperand *>(f->m_rOperand));
	}
    }
}

std::vector<IVariableOperand *> & BooleanExpression::variables()
{
  return m_variables;
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

BooleanTerm::~BooleanTerm()
{
  std::for_each(m_factors.begin(), m_factors.end(), free);
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

IBooleanFactor::~IBooleanFactor() 
{
}
