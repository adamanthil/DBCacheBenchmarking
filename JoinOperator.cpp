#include "JoinOperator.h"

void JoinOperator::concatSchema(const Schema * s1, const Schema * s2) {
	
	// Add attributes of 1st relation
	for(int i = 0; i < s1->size(); i++) {
		m_schema.add(s1->at(i));
	}
	
	// Add attributes of 2nd relation
	for(int i = 0; i < s2->size(); i++) {
		m_schema.add(s2->at(i));
	}
	
}

const Schema * JoinOperator::schema() const {
	return &m_schema;
}

JoinOperator::JoinOperator(IRelationalOperator * r1, IRelationalOperator * r2,
		JoinClause * clause) {
	m_child[0] = r1;
	m_child[1] = r2;
	m_clause = clause;
	concatSchema(r1->schema(), r2->schema());
}

JoinOperator::~JoinOperator() {
	// IRelationalOperator is responsible for deleting children
	delete m_child[LEFT];
	delete m_child[RIGHT];
}
