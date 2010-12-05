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
  BooleanExpression & m_expression;
  SelectionList & m_items;
  Tuple m_tuple;
 public:
  WhereClause(BooleanExpression &, SelectionList & items, 
	      std::vector<IVariableOperand *> &);
  ~WhereClause();

  bool evaluate(const Tuple & tuple);
  const SelectionList & filter() const;
  const Schema * schema() const;
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
