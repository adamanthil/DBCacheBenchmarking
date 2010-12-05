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

void SelectAll(Table & table, const Schema & schema)
{
  ProjectionList columns;

  columns.push_back(schema[0]);
  columns.push_back(schema[1]);
  //columns.push_back(schema[3]);

  IRelationalOperator * scan = new SequentialScan(table.path(), &schema);
  IRelationalOperator * proj = new Projection(scan, columns);

  proj->dump(std::cout);

  delete proj;
}

void CartesianJoin(Table & table1, Table & table2) 
{
	IRelationalOperator * scan1 = new SequentialScan(table1.path(), table1.schema());
	IRelationalOperator * scan2 = new SequentialScan(table2.path(), table2.schema());
	IRelationalOperator * loopJoin = new NestedBlockJoin(scan1, scan2, NULL);
	
	ProjectionList columns;
	for (int i = 0; i < loopJoin->schema()->nitems(); i++)
	{
		columns.push_back(loopJoin->schema()->at(i));
	}
	
	IRelationalOperator * projection = new Projection(loopJoin, columns);
	projection->dump(std::cout);

	delete projection;
}

void EquiJoin(Table & t1, Table & t2)
{

  ProjectionList columns;

  IRelationalOperator * scan1 = new SequentialScan(t1.path(), t1.schema());
  IRelationalOperator * scan2 = new SequentialScan(t2.path(), t2.schema());
  IRelationalOperator * join = new MergeJoin(scan1, scan2);

  for (int i = 0; i < join->schema()->nitems(); i++)
    {
      columns.push_back(join->schema()->at(i));
    }
  

  IRelationalOperator * projection = new Projection(join, columns);
  projection->dump(std::cout);

  delete projection;
  
}

void SelectWhere(Table & tbl)
{
 
  ProjectionList columns;
  SelectionList filter;

  filter.push_back(tbl.schema()->at(1));
  filter.push_back(tbl.schema()->at(0));

  for (int i = 0; i < tbl.schema()->nitems(); i++)
    {
      columns.push_back(tbl.schema()->at(i));
    }

  IntConstant l(25, INTEGER);
  IntVariable r(tbl.schema()->at(1), INTEGER);

  IntConstant l1(5, INTEGER);
  IntVariable r1(tbl.schema()->at(0), INTEGER);

  BooleanFactor<int> f(r, GE, l);
  BooleanFactor<int> f1(r1, LE, l1);
  BooleanTerm t;
  BooleanTerm t1;

  BooleanExpression exp(2);

  t.factor(&f);
  t.factor(&f1);
  exp.term(0, &t);
  //exp.term(1, &t1);

  std::vector<IVariableOperand *> vars;
  vars.push_back(&r);
  vars.push_back(&r1);

  WhereClause clause(exp, filter, vars);

  IRelationalOperator * scan = 
    new SequentialScan(tbl.path(), tbl.schema(), &clause);
  IRelationalOperator * proj = new Projection(scan, columns);
  proj->dump(std::cout, '|', '\n');

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
  Table * t0 = fm->getTable("test2");
  
  SelectAll(*t, *t->schema());
  SelectAll(*t0, *t0->schema());
  //  SelectWhere(*t);

  CartesianJoin(*t,*t);
  //EquiJoin(*t, *t0);
}

#endif
