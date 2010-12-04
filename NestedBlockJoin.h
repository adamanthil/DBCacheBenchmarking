#ifndef NESTED_BLOCK_JOIN_H_
#define NESTED_BLOCK_JOIN_H_

#include "IRelationalOperator.h"

class NestedBlockJoin : public IRelationalOperator
{
private:
	MemoryBlock * m_block;
	WhereClause * m_clause;
	Schema * m_schema;
	byte * m_data;	// Temporary storage for one record
	const IRelationalOperator * m_r1;
	const IRelationalOperator * m_r2;
	const MemoryBlock * m_leftBlock;	// Stores current block of left relation
	const MemoryBlock * m_rightBlock;	// Stores the current block of right relation
public:
	NestedBlockJoin(const IRelationalOperator * r1, const IRelationalOperator * r2,
			WhereClause * m_clause);
	~NestedBlockJoin();
	virtual const Schema * schema() const;
	virtual bool moveNext();
	virtual void next(MemoryBlock & block);
	virtual void reset();
};

#endif