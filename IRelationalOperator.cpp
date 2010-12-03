#include "IRelationalOperator.h"
#include <stdio.h>
#include <iostream>
bool IRelationalOperator::satisfies()
{
  return (m_boolExp != NULL ? 
	  m_boolExp->evaluate() : true);
}
