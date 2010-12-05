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

JoinClause::JoinClause(BooleanExpression & exp, const Schema * schema[],
	std::vector<IVariableOperand *> variables[]) : m_expression(exp) {
	m_tuple[LEFT].m_data = NULL;
	m_tuple[RIGHT].m_data = NULL;
	m_tuple[LEFT].schema(schema[LEFT]);
	m_tuple[RIGHT].schema(schema[RIGHT]);
	
		
}

const Schema * JoinClause::schema(int branch) const
{
  return m_tuple[branch].schema();
}

bool JoinClause::evaluate(const Tuple & t0, const Tuple & t1) {
	m_tuple[LEFT].m_data = t0.m_data;
	m_tuple[RIGHT].m_data = t1.m_data;
	return m_expression.evaluate();
}