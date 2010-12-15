#ifndef CLAUSE_H_
#define CLAUSE_H_

#include <vector>
#include "BooleanExpression.h"
#include "Tuple.h"
#include "Schema.h"

typedef std::vector<const Attribute *> SelectionList;

class WhereClause
{
 private:
  BooleanExpression * m_expression;
  Tuple m_tuple;
 public:
  WhereClause(BooleanExpression * expression);
  ~WhereClause();
  bool evaluate(const Tuple & tuple);
};

class JoinClause
{
 private:
  enum { LEFT = 0, RIGHT = 1 };

  BooleanExpression & m_expression;
  Tuple m_tuple[2];
  SelectionList m_items[2];
 public:
  JoinClause(BooleanExpression & exp, SelectionList items[],
	std::vector<IVariableOperand *> variables[]);
  ~JoinClause();
  bool evaluate(const Tuple & t0, const Tuple & t1);
  const SelectionList & filter(int branch) const;
  const Schema * schema(int branch) const;
};

#endif
