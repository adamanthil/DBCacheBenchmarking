#include 

class NestedBlockJoin : public IRelationalOperator
{
 public:
  NestedBlockJoin(IRelationalOperator & r1, IRelationalOperator r2,
		  Predicate * m_predicates);
};


