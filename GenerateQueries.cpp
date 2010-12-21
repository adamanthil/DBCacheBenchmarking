#ifndef GENERATE_QUERIES_H_
#define GENERATE_QUERIES_H_

#include <iostream> 
#include <sstream>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void query0(std::stringstream & text) {
	int number = rand() % 100 + 1;
	text << "sscan(T1:R; R.a, R.b; R.a < " << number << ")" << std::endl;
}

void query1(std::stringstream & text) {
	int number = rand() % 100 + 1;
	text << "sscan(T1:R; R.a, R.b; R.a > " << number << ")" << std::endl;
}

void genQuery(std::stringstream & text, int query) {
	switch(query) {
		case 0:
			query0(text);
			break;
		case 1:
			query1(text);
			break;
	}

}

// entry point
int main(int argc, char ** argv)
{
	int totalQueries;
	int * counts = new int[argc - 1];	// Counts of each type of query
	
	// Loop through number of arguments (number of queries to run)
	for(int i = 1; i < argc; i++) {
		int count = atoi(argv[i]);
		counts[i - 1] = count;
		totalQueries += count;
	}
	
	// Vector to store queries
	std::vector<std::stringstream*> * queries = new std::vector<std::stringstream*>();
	
	// initialize random seed
	srand ( time(NULL) );
	
	for(int i = 0; i < argc - 1; i++) {
		for(int j = 0; j < counts[i]; j++) {
			std::stringstream * text = new std::stringstream();
			genQuery(*text, i);
			queries->push_back(text);	
		}
	}
	
	// Randomize queries
	random_shuffle(queries->begin(), queries->end());
	
	// print queries to standard out
	for(int i = 0; i < totalQueries; i++) {
		std::cout << queries->at(i)->str();
		
		// clean up memory while we're at it
		delete queries->at(i);	
	}
	
	// Memory cleanup
	delete[] counts;
	delete queries;
	
	return 0;
}

#endif
