#ifndef OPERAND_H_
#define OPERAND_H_

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
  OperandType type;
};
  

#endif
