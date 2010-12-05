#include "JoinOperator.h"

Schema * JoinOperator::concatSchema(const Schema * s1, const Schema * s2) {
	Schema * schema = new Schema();
	
	// Add attributes of 1st relation
	for(int i = 0; i < s1->size(); i++) {
		schema->add(s1->at(i));
	}
	
	// Add attributes of 2nd relation
	for(int i = 0; i < s2->size(); i++) {
		schema->add(s2->at(i));
	}
	
	return schema;
}
