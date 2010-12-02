#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <stdio.h>
#include <iostream>

#include "Schema.h"
#include "FileManager.h"
#include "Attribute.h"
#include "Operators.h"

int main(int argc, char ** argv)
{

  Schema schema;

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

  FileManager * fm = FileManager::getInstance();
  IRelationalOperator * scan = new SequentialScan(std::string("Student"), 
						  &schema, &schema);
  IRelationalOperator * proj = new Projection(*scan, schema);

  proj->dump(std::cout);
}

#endif
