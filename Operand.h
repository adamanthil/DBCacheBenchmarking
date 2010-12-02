#ifndef OPERAND_H_
#define OPERAND_H_

#include "Attribute.h"
#include "globals.h"

template<typename T>
class Operand
{
 public:
  enum OperandType
  {
    COLUMN,
    CONSTANT
  };

  T value;
  T type;

 public:

  int compare(Operand other);
  //value(byte * buffer, const Attribute & attribute);
};
  

#endif
