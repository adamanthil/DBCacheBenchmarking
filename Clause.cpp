#include "Clause.h"

WhereClause::WhereClause(BooleanExpression & expression, const Schema * schema,
			 std::vector<IVariableOperand *> & variables) :
  m_expression(expression)
{
  m_tuple.m_data = NULL;
  m_tuple.schema(schema);

  for (int i = 0; i < variables.size(); i++)
    variables[i]->data(&m_tuple);    
}

const Schema * WhereClause::schema() const
{
  return m_tuple.schema();
}

bool WhereClause::evaluate(const Tuple & t)
{
  m_tuple.m_data = t.m_data;
  return m_expression.evaluate();
}

bool WhereClause::evaluate()
{
  return false;
}
