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
  columns.push_back(schema[3]);

  IRelationalOperator * scan = new SequentialScan(table.path(), &schema);
  IRelationalOperator * proj = new Projection(scan, columns);

  proj->dump(std::cout);

  delete proj;
}

void CartesianJoin(Table & table1, Table & table2) 
{
  /*
  IRelationalOperator * scan1 = new SequentialScan(table1.path(), table1.schema());
  IRelationalOperator * scan2 = new SequentialScan(table2.path(), table2.schema());
  IRelationalOperator * loopJoin = new NestedBlockJoin(scan1, scan2, NULL);
  IRelationalOperator * projection = new Projection(loopJoin, loopJoin->schema());
  projection->dump(std::cout);
	
  delete projection;
  delete loopJoin;
  delete scan2;
  delete scan1;
  */
}

void EquiJoin(Table & t1, Table & t2)
{
  /*
  IRelationalOperator * scan1 = new SequentialScan(t1.path(), t1.schema());
  IRelationalOperator * scan2 = new SequentialScan(t2.path(), t2.schema());
  IRelationalOperator * join = new MergeJoin(scan1, scan2);
  IRelationalOperator * projection = new Projection(join, join->schema());
  projection->dump(std::cout);

  delete projection;
  */
}

void SelectWhere(Table & tbl)
{
 
  ProjectionList columns;
  SelectionList filter;

  filter.push_back(tbl.schema()->at(1));

  for (int i = 0; i < tbl.schema()->nitems(); i++)
    {
      columns.push_back(tbl.schema()->at(i));
    }

  IntConstant l(39, INTEGER);
  IntVariable r(tbl.schema()->at(1), INTEGER);

  BooleanFactor<int> f(r, EQ, l);
  BooleanTerm t;
  BooleanExpression exp(1);

  t.factor(&f);
  exp.term(0, &t);

  std::vector<IVariableOperand *> vars;
  vars.push_back(&r);

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
  
  SelectAll(*t, *t->schema());
  SelectWhere(*t);

  //CartesianJoin(*t,*t);
  //EquiJoin(*t, *t);
}

#endif
