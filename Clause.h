#ifndef CLAUSE_H_
#define CLAUSE_H_

#include <vector>
#include "BooleanExpression.h"
#include "Tuple.h"
#include "Schema.h"


class WhereClause
{
 private:
  BooleanExpression & m_expression;
  Tuple m_tuple;
 public:
  WhereClause(BooleanExpression &, const Schema *, 
	      std::vector<IVariableOperand *> &);
  bool evaluate(const Tuple & tuple);
  const Schema * schema() const;
  bool evaluate();
};

class JoinClause
{
 private:
  enum { LEFT = 0, RIGHT = 1 };

  BooleanExpression & m_expression;
  Tuple m_tuple[2];
 public:
  JoinClause(BooleanExpression & exp, const Schema * schema[],
	std::vector<IVariableOperand *> variables[]);
  bool evaluate(const Tuple & t0, const Tuple & t1);
  const Schema * schema(int branch) const;
};

#endif
