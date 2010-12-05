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

  /* TODO: must be a better way todo this update the references */
  for (int i = 0; i < variables.size(); i++)
    {
      variables[i]->data(&m_tuple);
      const Attribute * attribute = variables[i]->attribute();
      for (int j = 0; j < schema->nitems(); j++)
	{
	  if (schema->at(j)->id() == attribute->id())
	    {
	      variables[i]->attribute(schema->at(j));
	      break;
	    }
	}
    }
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

JoinClause::JoinClause(BooleanExpression & exp, SelectionList items[],
		       std::vector<IVariableOperand *> variables[]) 
  : m_expression(exp) 
{
	
	for(int i = 0; i < 2; i++) {
		Schema * schema = new Schema();
		for(int j = 0; j < items[i].size(); j++) {
			schema->add(items[i].at(j));
		}
		m_tuple[i].m_data = NULL;
		m_tuple[i].schema(schema);
		m_items[i] = items[i];
	}
}

JoinClause::~JoinClause() {
	for(int i = 0; i < 2; i++) {
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
