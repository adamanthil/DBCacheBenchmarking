#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <stdio.h>
#include <iostream>

#include "FileManager.h"
#include "Attribute.h"
#include "Operators.h"

int main(int argc, char ** argv)
{

  std::vector<Attribute *> attributes;

  attributes.push_back(new Attribute(0, "id", "Student",
				     4, INTEGER));
  attributes.push_back(new Attribute(1, "ssn", "Student",
				     10, STRING));
  attributes.push_back(new Attribute(2, "fname", "Student",
				     20, STRING));
  attributes.push_back(new Attribute(3, "lname", "Student",
				     20, STRING));
  attributes.push_back(new Attribute(4, "year", "Student",
				     2, STRING));

  FileManager * fm = FileManager::getInstance();
  IRelationalOperator * scan = new SequentialScan(std::string("Student"), 
						  attributes);
  IRelationalOperator * proj = new Projection(*scan, attributes);

  proj->dump(std::cout);
}

#endif
