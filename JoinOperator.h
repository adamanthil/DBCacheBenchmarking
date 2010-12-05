#ifndef JOIN_OPERATOR_H_
#define JOIN_OPERATOR_H_

#include "IRelationalOperator.h"

class JoinOperator : public IRelationalOperator {
protected:
	// Creates a new schema as a concatenation of 2 others
	Schema * concatSchema(const Schema * s1, const Schema * s2);
public:
	virtual const Schema * schema() const = 0;
	virtual bool moveNext() = 0;
	virtual void next(MemoryBlock & block) = 0;
	virtual void reset() = 0;
};

#endif
