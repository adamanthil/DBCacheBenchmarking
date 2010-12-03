#ifndef BOOLEAN_FACTOR_H_
#define BOOLEAN_FACTOR_H_

#include <cstdlib>

#include "Operand.h"

enum LogicalOperator 
  {
    EQ = 0,
    LT,
    LE,
    GT,
    GE,
    NE
  };

class IBoolean
{
 public:
  virtual bool value() = 0;
};

class IBooleanFactor : public IBoolean
{
};

template<typename T>
class BooleanFactor : public IBooleanFactor
{
 private:
  IOperand<T> & m_lOperand;
  IOperand<T> & m_rOperand;
  LogicalOperator m_op;
 public:
  BooleanFactor(IOperand<T> & lOperand, LogicalOperator op, IOperand<T> & rOperand);
  virtual bool value();
};

class BooleanTerm : public IBoolean
{
 private:
  std::vector<IBooleanFactor *> m_factors;
 public:
  virtual bool value();
  void factor(IBooleanFactor * f);
  size_t nfactors() const;
};

class BooleanExpression
{
 private:
  BooleanTerm ** m_clauses;
  unsigned int m_ndisjuncts;
 public:
  BooleanExpression(unsigned int ndisjuncts);
  ~BooleanExpression();
  void term(unsigned int disjunct, BooleanTerm * t);
  bool evaluate();
};

template<typename T>
BooleanFactor<T>::BooleanFactor(IOperand<T> & lOperand, LogicalOperator op, IOperand<T> & rOperand)
: m_lOperand(lOperand), m_op(op), m_rOperand(rOperand)
{
}

template<typename T>
bool BooleanFactor<T>::value() 
{
  switch (m_op)
    {
    case EQ: return m_lOperand.compareTo(m_rOperand) == 0;
    case NE: return m_lOperand.compareTo(m_rOperand) != 0;
    case LT: return m_lOperand.compareTo(m_rOperand) < 0;
    case LE: return m_lOperand.compareTo(m_rOperand) <= 0;
    case GT: return m_lOperand.compareTo(m_rOperand) > 0;
    case GE: return m_lOperand.compareTo(m_rOperand) >= 0;
    }

  return false;
}


#endif
