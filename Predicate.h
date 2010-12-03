#ifndef PREDICATE_H_
#define PREDICATE_H_

#include <vector>
#include "Tuple.h"
#include "Operand.h"

class IPredicate 
{
 public:
  virtual bool eval() = 0;
  enum operator_t
  {
    EQ = 0,
    NE,
    LT,
    LE,
    GT,
    GE
  };
};

template<typename T>
class Predicate : public IPredicate
{
 public:
  IOperand<T> * m_lOperand;
  IOperand<T> * m_rOperand;
  operator_t m_op;
 public:
  Predicate(IOperand<T> * lOperand, IOperand<T> * rOperand, operator_t op);
  
  const IOperand<T> leftOperand() const;
  const IOperand<T> rightOperand() const;
  virtual bool eval();
};

class BooleanExpression
{
 private:
  int m_ndisjuncts;
  std::vector<IPredicate *> * m_predicates; // cnf. 
  std::vector<IVariableOperand *> m_variables;
 public:
  BooleanExpression(int ndisjuncts);
  ~BooleanExpression(); 
  void factor(int disjunct, IPredicate * p);
  bool evaluate();
  const std::vector<IVariableOperand *> * variables();
};

template<typename T>
Predicate<T>::Predicate(IOperand<T> * lOperand, IOperand<T> * rOperand,
			IPredicate::operator_t op) : m_op(op)
{
  m_lOperand = lOperand;
  m_rOperand = rOperand;
}

template<typename T>
bool Predicate<T>::eval()
{
  switch (m_op)
    {
    case EQ:
      return m_lOperand->compareTo(m_rOperand) == 0;
    case NE:
      return m_lOperand->compareTo(m_rOperand) != 0;
    case LT:
      return m_lOperand->compareTo(m_rOperand) < 0;
    case LE:
      return m_lOperand->compareTo(m_rOperand) <= 0;
    case GT:
      return m_lOperand->compareTo(m_rOperand) > 0;
    case GE:
      return m_lOperand->compareTo(m_rOperand) >= 0;
    default:
      return false;
    }
}

#endif
