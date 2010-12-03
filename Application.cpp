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
#include "Predicate.h"

int main(int argc, char ** argv)
{

  Schema schema;
  Schema filterSchema;

  schema.add(new Attribute(1, "id", "Student",
			   10, INTEGER));
  schema.add(new Attribute(1, "ssn", "Student",
			   10, STRING));
  schema.add(new Attribute(2, "fname", "Student",
			   20, STRING));
  schema.add(new Attribute(3, "lname", "Student",
			   20, STRING));
  schema.add(new Attribute(4, "year", "Student",
			   2, STRING));
  filterSchema.add(schema.at(0));

  typedef ConstantOperand<int> IntOperand;
  
  ConstantOperand<int> lOperand(0, INTEGER);
  VariableOperand<int> rOperand(schema.at(0), INTEGER);
  Predicate<int> p(&lOperand, &rOperand, IPredicate::EQ);

  BooleanExpression bexp(1);
  bexp.factor(0, &p);
 
  FileManager * fm = FileManager::getInstance();
  
  IRelationalOperator * scan = new SequentialScan(std::string("Student"), 
						  &schema);
  IRelationalOperator * proj = new Projection(*scan, schema);

  proj->dump(std::cout);
  

  delete scan;
  delete proj;

  scan = new SequentialScan(std::string("Student"), &schema,  
			    &filterSchema, &bexp);
						  
  proj = new Projection(*scan, schema);

  proj->dump(std::cout);
  
}

#endif
