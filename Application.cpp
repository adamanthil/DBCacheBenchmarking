#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <stdio.h>
#include <iostream>
#include <new>

#include "Database.h"
#include "FileManager.h"
#include "BufferManager.h"
#include "Query.h"

#include "Schema.h"
#include "Attribute.h"
#include "Operators.h"

#include "Operand.h"
#include "BooleanExpression.h"
#include "DataCreator.h"

typedef ConstantOperand<int> IntConstant;
typedef ConstantOperand<const char *> StringConstant;

typedef VariableOperand<int> IntVariable;
typedef VariableOperand<const char *> StringVariable;

void SelectAll(const Table & table)
{
  ProjectionList columns;

  for (int i = 0; i < table.schema()->nitems(); i++)
    {
      columns.push_back(table.schema()->at(i));
    }

  IRelationalOperator * scan = new SequentialScan(table.path(), table.schema());
  IRelationalOperator * proj = new Projection(scan, columns);

  proj->dump(std::cout);

  /*
    Query q(1, proj);
      
      std::cerr << "started select all: ";
      q.profile();
      q.stats(std::cout);
  */
}

void ProjectionFilter(const Table & table)
{
  ProjectionList columns;
  columns.push_back(table.schema()->at(0));
  columns.push_back(table.schema()->at(1));
  columns.push_back(table.schema()->at(8));

  IRelationalOperator * scan = new SequentialScan(table.path(), table.schema());
  IRelationalOperator * proj = new Projection(scan, columns);

  proj->dump(std::cout);
}

void LoopJoin(const Table & table1, const Table & table2) 
{
  /*
  int joinCol1 = 0;
  int joinCol2 = 0;
	
  BooleanExpression bexp(1);
	
  IntVariable r1(table1.schema()->at(joinCol1), INTEGER);
  IntVariable r2(table2.schema()->at(joinCol2), INTEGER);
  BooleanFactor<int> f(r1, EQ, r2);
  BooleanTerm t;
	
  t.factor(&f);
	
  bexp.term(0,&t);
	
  SelectionList filter[2];
  filter[0].push_back(table1.schema()->at(joinCol1));
  filter[1].push_back(table1.schema()->at(joinCol2));
	
  std::vector<IVariableOperand *> variables[2];
	
  variables[0].push_back(&r1);
  variables[1].push_back(&r2);
	
  JoinClause joinClause(bexp, filter, variables);
	
  IRelationalOperator * scan1 = new SequentialScan(table1.path(), table1.schema());
  IRelationalOperator * scan2 = new SequentialScan(table2.path(), table2.schema());
  IRelationalOperator * loopJoin = new NestedBlockJoin(scan1, scan2, &joinClause);
	
  ProjectionList columns;
  for (int i = 0; i < loopJoin->schema()->nitems(); i++)
    {
      columns.push_back(loopJoin->schema()->at(i));
    }
	
  IRelationalOperator * projection = new Projection(loopJoin, columns);
  projection->dump(std::cout);

  delete projection;
  */
}

void CartesianJoin(const Table & table1, const Table & table2) 
{
  /*
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
  */
}

void EquiJoin(const Table & t1, const Table & t2)
{

  /*
  ProjectionList columns;

  Schema scan1Schema;
  scan1Schema.add(t1.schema()->at(0));
  scan1Schema.add(t1.schema()->at(8));

  Schema scan2Schema;
  scan2Schema.add(t2.schema()->at(1));
  scan2Schema.add(t2.schema()->at(2));

  IRelationalOperator * scan1 = new SequentialScan(t1.path(), &scan1Schema);
  IRelationalOperator * scan2 = new SequentialScan(t2.path(), &scan2Schema);
  IRelationalOperator * join = new MergeJoin(scan1, scan2);

  for (int i = 0; i < join->schema()->nitems(); i++)
    {
      if (i != 2)
      columns.push_back(join->schema()->at(i));
    }  

  IRelationalOperator * projection = new Projection(join, columns);

  Query q(1, projection);
  q.profile();
  q.stats(std::cout);
  */
  /*
  projection->dump(std::cout);

  delete projection;
  */
}

void SelectWhere(const Table & tbl)
{
 
  /*
  ProjectionList columns;
  SelectionList filter;

  filter.push_back(tbl.schema()->at(8));
  //  filter.push_back(tbl.schema()->at(9));

  columns.push_back(tbl.schema()->at(0));
  columns.push_back(tbl.schema()->at(8));

  IntConstant l(25, INTEGER);
  IntVariable r(tbl.schema()->at(8), INTEGER);

  IntConstant l1(63, INTEGER);
  IntVariable r1(tbl.schema()->at(8), INTEGER);

  BooleanFactor<int> f(r, GE, l);
  BooleanFactor<int> f1(r1, LE, l1);
  BooleanTerm t;

  BooleanExpression exp(1);

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

  Query q(1, proj);

  std::cerr << "started select where: ";
  //q.profile();
  //q.stats(std::cout);

  // delete proj;

   proj->dump(std::cout, '|', '\n');
  */
}

int main(int argc, char ** argv)
{
  Schema schema;
  Schema projection;
  
  std::cerr << "initializing...";
  //DataCreator::CreateDB("createdb", false);
  
  BufferManager::Initialize(4096);
  FileManager::Initialize(argv[1], "db.xml");
  Database::Initialize("db.xml");
  Database * db = Database::getInstance();
  
  std::cerr << "complete" << std::endl;

  const Table * t = db->table("test1");
  const Table * t0 = db->table("test2");
  
  // SelectAll(*t);
  ProjectionFilter(*t);
  //SelectAll(*t0);
  //SelectWhere(*t);

  //CartesianJoin(*t,*t0);
  //EquiJoin(*t, *t0);
  //LoopJoin(*t,*t);

}

#endif
