#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <stdio.h>
#include <iostream>
#include <new>

#include "Schema.h"
#include "FileManager.h"
#include "BufferManager.h"
#include "Attribute.h"
#include "Operators.h"

#include "Operand.h"
#include "BooleanExpression.h"
#include "DataCreator.h"

typedef ConstantOperand<int> IntConstant;
typedef ConstantOperand<const char *> StringConstant;

typedef VariableOperand<int> IntVariable;
typedef VariableOperand<const char *> StringVariable;

void SelectAll(Table & table, const Schema & schema, const Schema & projection)
{
  IRelationalOperator * scan = new SequentialScan(table.path(), &schema);
  IRelationalOperator * proj = new Projection(*scan, &projection);

  proj->dump(std::cout);

  delete scan;
  delete proj;
}

void CartesianJoin(Table & table1, Table & table2) {
	IRelationalOperator * scan1 = new SequentialScan(table1.path(), table1.schema());
	IRelationalOperator * scan2 = new SequentialScan(table2.path(), table2.schema());
	IRelationalOperator * loopJoin = new NestedBlockJoin(scan1, scan2, NULL);
	IRelationalOperator * projection = new Projection(*loopJoin, loopJoin->schema());
	projection->dump(std::cout);
	
	delete projection;
	delete loopJoin;
	delete scan2;
	delete scan1;
}

void SelectWhere(Schema & schema, int field)
{
  Schema filter;
  
  IntConstant l0(2, INTEGER);
  IntVariable r0(schema["id"], INTEGER);

  StringConstant l1("Greig               ", STRING);
  StringVariable r1(schema["fname"], STRING);

  BooleanFactor<int> f0(r0, LT, l0);
  BooleanFactor<const char *> f1(l1, EQ, r1);
  BooleanTerm t0;
  BooleanTerm t1;
  BooleanExpression exp(2);
  std::vector<IVariableOperand *> vars;

  filter.add(schema.at(0));
  filter.add(schema.at(2));

  t1.factor(&f1);
  t0.factor(&f0);

  exp.term(0, &t1);
  exp.term(1, &t0);

  vars.push_back(&r0);
  vars.push_back(&r1);

  WhereClause clause(exp, &filter, vars);

  IRelationalOperator * scan = 
    new SequentialScan(std::string("Student.tab"), &schema, &clause);
  IRelationalOperator * proj = new Projection(*scan, &schema);
  proj->dump(std::cout, '|', '\n');

  delete scan;
  delete proj;
  
}

int main(int argc, char ** argv)
{
  Schema schema;
  Schema projection;
  
  DataCreator::CreateDB("createdb", true);

  BufferManager::Initialize();
  FileManager::Initialize("config", "db.xml");

  FileManager * fm = FileManager::getInstance();
  Table * t = fm->getTable("test1");
  
  SelectAll(*t, *t->schema(), *t->schema());

  CartesianJoin(*t,*t);
  //SelectWhere(schema, 2);
  
}

#endif
