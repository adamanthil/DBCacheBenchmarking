#ifndef PREDICATE_H_
#define PREDICATE_H_

#include "Tuple.h"
#include "Operand.h"

class IPredicate 
{
 public:
  virtual bool eval(const Tuple & tuple) = 0;
};

template<class T, class K>
class Predicate : public IPredicate
{
 public:
  enum operator_t
  {
    EQ = 0,
    NE,
    LT,
    LE,
    GT,
    GE
  };

 protected:
  Operand<T> & m_lOperand;
  Operand<K> & m_rOperand;
  operator_t m_op;
 public:

  Predicate(Operand<T> & lOperand, Operand<K> & mOperand, 
	    operator_t op);
  virtual bool eval(const Tuple & t);
};

/*
class JoinPredicate : public Predicate<int>
{

 public:
  bool eval(const Tuple &, const Tuple &);
};
*/

class BooleanExpression
{
 private:
  IPredicate * m_predicates[];
 public:
  bool evaluate(const Tuple & t);
  bool evaluate(const Tuple &, const Tuple &);
};

#endif
