#ifndef NESTED_BLOCK_JOIN_H_
#define NESTED_BLOCK_JOIN_H_

#include "IRelationalOperator.h"

class NestedBlockJoin : public IRelationalOperator
{
private:
	MemoryBlock * m_block;
	IRelationalOperator * m_r1;
	IRelationalOperator * m_r2;
public:
	NestedBlockJoin(IRelationalOperator & r1, IRelationalOperator & r2,
			BooleanExpression & m_boolExp);
	~NestedBlockJoin();
	virtual bool moveNext();
	virtual void next(MemoryBlock & block);
	virtual void reset();
};

#endif