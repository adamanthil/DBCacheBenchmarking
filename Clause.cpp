#include "Clause.h"

WhereClause::WhereClause(BooleanExpression & expression, 
			 SelectionList & items,
			 std::vector<IVariableOperand *> & variables) :
  m_expression(expression), m_items(items)
{

  Schema * schema = new Schema();
  for (int i = 0; i < items.size(); i++)
    schema->add(items[i]);

  m_tuple.m_data = NULL;
  m_tuple.schema(schema);

  for (int i = 0; i < variables.size(); i++)
    variables[i]->data(&m_tuple);
}

WhereClause::~WhereClause()
{
  delete m_tuple.schema();
}

const SelectionList & WhereClause::filter() const
{
  return m_items;
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

JoinClause::JoinClause(BooleanExpression & exp, const Schema * schema[],
		       std::vector<IVariableOperand *> variables[]) 
  : m_expression(exp) 
{
  m_tuple[LEFT].m_data = NULL;
  m_tuple[RIGHT].m_data = NULL;
  m_tuple[LEFT].schema(schema[LEFT]);
  m_tuple[RIGHT].schema(schema[RIGHT]);
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
