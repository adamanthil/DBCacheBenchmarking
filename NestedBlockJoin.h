#ifndef NESTED_BLOCK_JOIN_H_
#define NESTED_BLOCK_JOIN_H_

#include "JoinOperator.h"
#include "Clause.h"
#include "Tuple.h"

class NestedBlockJoin : public JoinOperator
{
private:
	MemoryBlock * m_buffer;
	WhereClause * m_clause;
	Schema * m_schema;
	byte * m_data;	// Temporary storage for one record
	IRelationalOperator * m_r1;
	IRelationalOperator * m_r2;
	const MemoryBlock * m_leftBlock;	// Stores current block of left relation
	const MemoryBlock * m_rightBlock;	// Stores the current block of right relation
	int m_leftBlockRid;					// Position in the left block
	int m_rightBlockRid;				// Position in the right block
	
	// Reads in the next left Tuple to compare
	Tuple * nextLeftTuple(Tuple * tuple);
	
	// Reads in the next right Tuple to compare
	Tuple * nextRightTuple(Tuple * tuple);
	
public:
	NestedBlockJoin(IRelationalOperator * r1, IRelationalOperator * r2,
			WhereClause * m_clause);
	~NestedBlockJoin();
	virtual const Schema * schema() const;
	virtual bool moveNext();
	virtual void next(MemoryBlock & block);
	virtual void reset();
};

#endif
