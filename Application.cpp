#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <stdio.h>
#include <iostream>
#include <new>

#include "Schema.h"
#include "FileManager.h"
#include "Attribute.h"
#include "Operators.h"

#include "Operand.h"
#include "BooleanExpression.h"

typedef ConstantOperand<int> IntConstant;
typedef ConstantOperand<const char *> StringConstant;

typedef VariableOperand<int> IntVariable;
typedef VariableOperand<const char *> StringVariable;

void SelectAll(Schema & schema)
{
  IRelationalOperator * scan = new SequentialScan(std::string("Student"), 
						  &schema);
  IRelationalOperator * proj = new Projection(*scan, schema);

  proj->dump(std::cout);

  delete scan;
  delete proj;
}

void SelectWhere(Schema & schema, int field)
{
  Schema filter;
  
  IntConstant lOperand(0, INTEGER);
  IntVariable rOperand(schema.at(0), INTEGER);

  StringConstant lOperand2("Greig", STRING);
  StringVariable rOperand2(schema.at(2), STRING);

  BooleanFactor<int> f(lOperand, EQ, rOperand);
  BooleanTerm t;
  BooleanExpression exp(1);
  std::vector<IVariableOperand *> vars;


  filter.add(schema.at(field));

  t.factor(&f);
  exp.term(0, &t);
  vars.push_back(&rOperand);

  WhereClause clause(exp, &filter, vars);

  IRelationalOperator * scan = 
    new SequentialScan(std::string("Student"), &schema, &clause);
  IRelationalOperator * proj = new Projection(*scan, schema);
  proj->dump(std::cout);

  delete scan;
  delete proj;
}

int main(int argc, char ** argv)
{
  Schema schema;

  schema.add(new Attribute(1, "id", "Student", 4, INTEGER));
  schema.add(new Attribute(1, "ssn", "Student", 10, STRING));
  schema.add(new Attribute(2, "fname", "Student", 20, STRING));
  schema.add(new Attribute(3, "lname", "Student", 20, STRING));
  schema.add(new Attribute(4, "year", "Student", 2, STRING));

  SelectAll(schema);
  SelectWhere(schema, 0);
}

#endif
