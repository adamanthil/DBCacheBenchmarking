#ifndef GENERATE_QUERIES_H_
#define GENERATE_QUERIES_H_

#include <iostream> 
#include <sstream>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char * getOperator(int index) {
	switch(index) {
		case 0:
		return "<";
		case 1:
		return "=";
		default:
		return ">";
	}
}

// Template: sscan(T1:t; t.a, t.b, t.n, t.o; )
void query0(std::stringstream & text) {
	text << "sscan(T1:t; t.a, t.b, t.n, t.o; )" << std::endl;
}

// Template: sscan(T1:t; t.a, t.b, t.n, t.o; t.a < 500 | t.a > 4800 & t.a < 5300 | t.a > 97000)
void query1(std::stringstream & text) {
	int number1 = rand() % 500 + 1;
	int number2 = 2000 + (rand() % 3000 + 1);
	int number3 = number2 + (rand() % 1000 + 1);
	int number4 = 70000 + (rand() % 20000 + 1);
	text << "sscan(T1:t; t.a, t.b, t.n, t.o; t.a < " << number1 
		<< " | t.a > " << number2 << " & t.a < " << number3 << " | t.a > " << number4 << ")" << std::endl;
}

// Template: sscan(T1:t; t.l, t.m, t.q; t.l < t.m)
void query2(std::stringstream & text) {
	int random = rand() % 3;
	const char * sign = getOperator(random);
	text << "sscan(T1:t; t.l, t.m, t.q; t.l " << sign << " t.m)" << std::endl;
}

// Template: sscan(T2:t; t.b, t.l, t.m; t.b < 500 | t.b > 3300 & t.b < 4573 | t.b > 98000)
void query3(std::stringstream & text) {
	const char * sign1 = getOperator(rand() % 3);
	const char * sign2 = getOperator(rand() % 3);
	int number1 = rand() % 500 + 1;
	int number2 = 80000 + (rand() % 20000 + 1);
	text << "sscan(T2:t; t.b, t.l, t.m; t.b " << sign1 << " " << number1 << " | t.b  > " 
		<< "3300 & t.b < 4573 | t.b " << sign2 << " " << number2 << ")" << std::endl;
}

// Template: sscan(T2:t; t.b, t.m, t.l; t.l > t.m)
void query4(std::stringstream & text) {
	int random = rand() % 3;
	const char * sign = getOperator(random);
	text << "sscan(T2:t; t.b, t.m, t.l; t.l " << sign << " t.m)" << std::endl;
}

// Template: sscan(T3:t; t.b, t.m, t.q; t.m = ‘A’ | t.m = ‘E’ | t.m = ‘I’ | t.m = ‘O’ | t.m = ‘U’)
void query5(std::stringstream & text) {
	char char1 = (char)(rand() % 26 + 65);
	char char2 = (char)(rand() % 26 + 65);
	char char3 = (char)(rand() % 26 + 65);
	char char4 = (char)(rand() % 26 + 65);
	char char5 = (char)(rand() % 26 + 65);
	
	text << "sscan(T3:t; t.b, t.m, t.q; t.m = '" << char1 << "' | t.m = '" << char2 
		<< "' | t.m = '" << char3 << "' | t.m = '" << char4 << "' | t.m = '" << char5 << "')" << std::endl;
}

// Template: sscan(T3:t; t.a, t.h; t.a < 3800 | t.a > 50000 & t.h = ‘H’)
void query6(std::stringstream & text) {
	text << "sscan(T3:t; t.a, t.h; t.a < 3800 | t.a > 50000 & t.h = ‘H’)" << std::endl;
}

void genQuery(std::stringstream & text, int query) {
	switch(query) {
		case 0:
			query0(text);
			break;
		case 1:
			query1(text);
			break;
		case 2:
			query2(text);
			break;
		case 3:
			query3(text);
			break;
		case 4:
			query4(text);
			break;
		case 5:
			query5(text);
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
	
	std::stringstream * text = new std::stringstream();
	*text << "end" << std::endl;
	queries->push_back(text);
	
	// print queries to standard out
	for(int i = 0; i < totalQueries + 1; i++) {
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
