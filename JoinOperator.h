#ifndef JOIN_OPERATOR_H_
#define JOIN_OPERATOR_H_

#include "IRelationalOperator.h"
#include "Clause.h"

class JoinOperator : public IRelationalOperator {
protected:
	IRelationalOperator * m_child[2];
	enum { LEFT = 0, RIGHT = 1};
	
	JoinClause * m_clause;
	Schema m_schema;
	// Creates a new schema as a concatenation of 2 others
	void concatSchema(const Schema * s1, const Schema * s2);
public:
	JoinOperator(IRelationalOperator * r1, IRelationalOperator * r2,
			JoinClause * clause);
	~JoinOperator();
	virtual const Schema * schema() const;
	virtual bool moveNext() = 0;
	virtual void next(MemoryBlock & block) = 0;
	virtual void reset() = 0;
};

#endif