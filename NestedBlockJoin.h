#ifndef NESTED_BLOCK_JOIN_H_
#define NESTED_BLOCK_JOIN_H_

#include "JoinOperator.h"
#include "Clause.h"
#include "Tuple.h"

class NestedBlockJoin : public JoinOperator
{
private:
	MemoryBlock * m_buffer;
	byte * m_data;	// Temporary storage for one record
	MemoryBlock * m_childBuffer[2];	// Stores current block of child relations
	int m_next[2];					// Record position in child relation
	Tuple m_childTuple[2];			// Temporary storage for child tuples
	
	// Reads in the next Tuple to compare
	bool nextTuple(int branch, Tuple * tuple);
	
public:
	NestedBlockJoin(IRelationalOperator * r1, IRelationalOperator * r2,
			JoinClause * m_clause);
	~NestedBlockJoin();
	virtual bool moveNext();
	virtual void next(MemoryBlock & block);
	virtual void reset();
};

#endif