#ifndef CLAUSE_H_
#define CLAUSE_H_

#include <vector>
#include "BooleanExpression.h"
#include "Tuple.h"
#include "Schema.h"

class IClause
{
 public:
};

class WhereClause : public IClause
{
 private:
  BooleanExpression & m_expression;
  Tuple m_tuple;
 public:
  WhereClause(BooleanExpression &, const Schema *, 
	      std::vector<IVariableOperand *> &);
  bool evaluate(const Tuple & tuple);
  const Schema * schema() const;
  virtual bool evaluate();
};

class JoinClause : public IClause
{
 private:
  enum { LEFT = 0, RIGHT = 1 };

  BooleanExpression & m_expression;
  Tuple m_Tuple[2];
 public:
  bool evaluate(const Tuple & t0, const Tuple & t1);
};

#endif
