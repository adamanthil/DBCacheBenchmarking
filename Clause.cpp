#include "Clause.h"

WhereClause::WhereClause(BooleanExpression * expression) :
  m_expression(expression)
{
  for (int i = 0; i < m_expression->variables().size(); i++)
    {
      /* set location of the data */
      m_expression->variables()[i]->data(&m_tuple);
    }
}

WhereClause::~WhereClause()
{
  delete m_expression;
}

bool WhereClause::evaluate(const Tuple & t)
{
  m_tuple.schema(t.schema());
  m_tuple.m_data = t.m_data;
  return m_expression->evaluate();
}

JoinClause::JoinClause(BooleanExpression & exp, SelectionList items[],
		       std::vector<IVariableOperand *> variables[]) 
  : m_expression(exp) 
{
	
  for(int i = 0; i < 2; i++) 
    {
      Schema * schema = new Schema();

      m_tuple[i].m_data = NULL;
      m_tuple[i].schema(schema);
      m_items[i] = items[i];
      
      for(int j = 0; j < items[i].size(); j++) 
	{
	  schema->add(items[i].at(j));
	}
      
      for (int j = 0; j < variables[i].size(); j++)
	{
	  /* set location of the data */
	  variables[i].at(j)->data(&m_tuple[i]);	  
	}
    }
}

JoinClause::~JoinClause() 
{
  for(int i = 0; i < 2; i++) 
    {
      delete m_tuple[i].schema();
    }
}

const Schema * JoinClause::schema(int branch) const
{
  return m_tuple[branch].schema();
}

bool JoinClause::evaluate(const Tuple & t0, const Tuple & t1) 
{
  m_tuple[LEFT].m_data = t0.m_data;
  m_tuple[RIGHT].m_data = t1.m_data;
  return m_expression.evaluate();
}

const SelectionList & JoinClause::filter(int branch) const 
{
  return m_items[branch];
}
