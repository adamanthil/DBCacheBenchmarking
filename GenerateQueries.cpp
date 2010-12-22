#ifndef GENERATE_QUERIES_H_
#define GENERATE_QUERIES_H_

#include <iostream> 
#include <sstream>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Number of optimized queries (ugly and hardcoded since the queries are hardcoded - sadface )
const int numOptimized = 15;

const char * getOperator(int index) {
	switch(index) {
		case 0:
			return "<";
		case 1:
			return ">";
		case 3: // Skip 2 so that != is default.  EQ will only be returned if index is >= 3
			return "=";
		default:
			return "!=";
	}
}

// ----------------------------------------------------------
// Queries Optimized for Partition Scheme
// ----------------------------------------------------------
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
	const char * op = getOperator(random);
	text << "sscan(T1:t; t.l, t.m, t.q; t.l " << op << " t.m)" << std::endl;
}

// Template: sscan(T2:t; t.b, t.l, t.m; t.b < 500 | t.b > 3300 & t.b < 4573 | t.b > 98000)
void query3(std::stringstream & text) {
	const char * op1 = getOperator(rand() % 4);
	const char * op2 = getOperator(rand() % 4);
	int number1 = rand() % 500 + 1;
	int number2 = 80000 + (rand() % 20000 + 1);
	text << "sscan(T2:t; t.b, t.l, t.m; t.b " << op1 << " " << number1 << " | t.b  > " 
		<< "3300 & t.b < 4573 | t.b " << op2 << " " << number2 << ")" << std::endl;
}

// Template: sscan(T2:t; t.b, t.m, t.l; t.l > t.m)
void query4(std::stringstream & text) {
	int random = rand() % 4;
	const char * op = getOperator(random);
	text << "sscan(T2:t; t.b, t.m, t.l; t.l " << op << " t.m)" << std::endl;
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

// Template: sscan(T3:t; t.a, t.h; t.a < 3800 | t.a > 50000 & t.h = 'H')
void query6(std::stringstream & text) {
	int number1 = rand() % 4000 + 1;
	int number2 = 40000 + (rand() % 1000 + 1);
	char char1 = (char)(rand() % 26 + 65);
	text << "sscan(T3:t; t.a, t.h; t.a < " << number1 << " | t.a > " << number2 << " & t.h = '" << char1 << "')" << std::endl;
}

// Template: sscan(T4:t; t.a, t.b, t.c, t.l, t.m, t.p, t.n, t.o; )
void query7(std::stringstream & text) {
	text << "sscan(T4:t; t.a, t.b, t.c, t.l, t.m, t.p, t.n, t.o; )" << std::endl;
}

// Template: sscan(T4:t; t.a, t.f, t.g, t.i, t.m, t.n, t.n, t.o, t.q, t.r; t.m < t.n)
void query8(std::stringstream & text) {
	const char * op1 = getOperator(rand() % 3);
	text << "sscan(T4:t; t.a, t.f, t.g, t.i, t.m, t.n, t.n, t.o, t.q; t.m " << op1 << " t.n)" << std::endl;
}

// Template: sscan(T5:t; t.l, t.m, t.n, t.q; )
void query9(std::stringstream & text) {
	text << "sscan(T5:t; t.l, t.m, t.n, t.q; )" << std::endl;
}

// Template: sscan(T5:t; t.l, t.m, t.n, t.q; t.m > t.n & t.l = 'Z')
void query10(std::stringstream & text) {
	char char1 = (char)(rand() % 26 + 65);
	const char * op1 = getOperator(rand() % 3);
	text << "sscan(T5:t; t.l, t.m, t.n, t.q; t.m " << op1 << " t.n & t.l = '" << char1 << "')" << std::endl;
}

// Template: merge-join(sscan(T1:R; R.a, R.b, T.n, R.o; ), sscan(T3:S; S.a, S.h, S.r; S.h = 'A' | S.h = 'B'); R.a = S.a; )
void query11(std::stringstream & text) {
	char char1 = (char)(rand() % 26 + 65);
	char char2 = (char)(rand() % 26 + 65);
	text << "merge-join(sscan(T1:R; R.a, R.b, T.n, R.o; ), sscan(T3:S; S.a, S.h, S.r; S.h = '" 
		<< char1 << "' | S.h = '" << char2 <<"'); R.a = S.a; )" << std::endl;
}

// Template: merge-join(sscan(T1:R; R.a, R.b, T.n, R.o; R.a < 5500), sscan(T3:S; S.a, S.h, S.r; S.h = 'A' | S.h = 'B'); R.a = S.a; )
void query12(std::stringstream & text) {
	int number = rand() % 10000 + 1;
	char char1 = (char)(rand() % 26 + 65);
	char char2 = (char)(rand() % 26 + 65);
	text << "merge-join(sscan(T1:R; R.a, R.b, T.n, R.o; R.a < " << number 
		<< "), sscan(T3:S; S.a, S.h, S.r; S.h = '" << char1 << "' | S.h = '" << char2 << "'); R.a = S.a; )" << std::endl;
}

// Template: merge-join(sscan(T3:R; R.a, R.r; ), sscan(T5:S; S.f, S.e, S.p; S.e < 5); R.a = S.f; )
void query13(std::stringstream & text)
{
  const char * op = getOperator(rand() % 4);
  int number = rand() % 10 + 1;
  text << "merge-join(sscan(T3:R; R.a, R.r; ), sscan(T5:S; S.f, S.e, S.p; S.e " << op << " " << number << "); R.a = S.f; )" << std::endl;
}


//  Template: merge-join(merge-join(sscan(T1:S; S.a, S.b, S.o; ), sscan(T3:R; R.b, R.m, R.q; R.b >= 4000 & R.m < 'F'); R.b = S.a;), sscan(T5:T; T.d, T.f; ); T.f = R.a; )
void query14(std::stringstream & text)
{
	int number = rand() % 10000 + 1;
	char char1 = (char)(rand() % 26 + 65);
	text << "merge-join(merge-join(sscan(T1:S; S.a, S.b, S.o; ), "
		<< "sscan(T3:R; R.b, R.m, R.q; R.b >= " << number << " & R.m < '" << char1 << "'); R.b = S.a;)," 
		<< " sscan(T5:T; T.d, T.f; ); T.f = R.a; )" << std::endl;
}

// ----------------------------------------------------------
// Queries Not Optimized for Partition Scheme
// ----------------------------------------------------------

// Template: sscan(T1:t; t.a, t.n, t.c, t.e, t.m, t.q; )
void query15(std::stringstream & text) {
	text << "sscan(T1:t; t.a, t.n, t.c, t.e, t.m, t.q; )" << std::endl;
}

// Template: sscan(T3:t; t.a, t.h, t.f, t.c, t.q; T3.m = 'B')
void query16(std::stringstream & text) {
	const char * op = getOperator(rand() % 4);
	char char1 = (char)(rand() % 26 + 65);
	text << "sscan(T3:t; t.a, t.h, t.f, t.c, t.q, t.m; t.m " << op << " '" << char1 << "')" << std::endl;
}

void query17(std::stringstream & text) {
  const char * op = getOperator(rand() % 4);
  int number = rand() % 10 + 1;
  text << "merge-join(sscan(T5:R; R.a, R.o, R.f; ), sscan(T3:S; S.a, S.b, S.c, S.f, S.i, S.q, S.e; S.e " 
    << op << " " << number << "); R.a = S.f; )" << std::endl;
}

void query18(std::stringstream & text) {
  const char * op1 = getOperator(rand() % 4);
  const char * op2 = getOperator(rand() % 4);
  char char1 = (char)(rand() % 26 + 65);
  int number = rand() % 10 + 1;
  text << "merge-join(sscan(T4:R; R.a, R.o, R.f, R.l; R.l " << op1 << " '" << char1 << "'), sscan(T5:S; S.a, S.b, S.c, S.f, S.i, S.q, S.e; S.e " 
    << op2 << " " << number << "); R.a = S.f; )" << std::endl;
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
		case 6:
			query6(text);
			break;
		case 7:
			query7(text);
			break;
		case 8:
			query8(text);
			break;
		case 9:
			query9(text);
			break;
		case 10:
			query10(text);
			break;
		case 11:
			query11(text);
			break;
		case 12:
			query12(text);
			break;
		case 13:
			query13(text);
			break;
		case 14:
			query14(text);
			break;
			
		// Non partition-optimized queries
		case 15:
			query15(text);
			break;
		case 16:
			query16(text);
			break;
		case 17:
			query17(text);
			break;
		case 18:
			query18(text);
			break;
	}
}

// entry point
int main(int argc, char ** argv)
{
	// check arguments
	if(argc > numOptimized + 2) {
		std::cout << "Supply at most " << (numOptimized + 1) << " arguments: [ratio non-optimized:optimized queries], [numQ1], [numQ2], ..." << std::endl;
		exit(1);
	}
	
	if(!strcmp(argv[1], "--help") || !strcmp(argv[1], "help")) {
		std::cout << "Supply at most " << (numOptimized + 1) << " arguments: [ratio non-optimized:optimized queries], [numQ1], [numQ2], ..." << std::endl;
		exit(0);
	}
	
	int totalOptQueries = 0;	// Total number of optimized queries
	int * counts = new int[argc - 1];	// Counts of each type of query
	
	float ratio = atof(argv[1]);	// Ratio of non-optimized queries to optimized
	int totalNotOptQueries = 0;
	int totalQueries = 0;
	
	// Loop through number of arguments (number of queries to run)
	for(int i = 2; i < argc; i++) {
		int count = atoi(argv[i]);
		counts[i - 2] = count;
		totalOptQueries += count;
	}
	
	// Calculate number of non-optimzed queries to add
	totalNotOptQueries = (int)(ratio * totalOptQueries);
	totalQueries = totalOptQueries + totalNotOptQueries;
	
	// Vector to store queries
	std::vector<std::stringstream*> * queries = new std::vector<std::stringstream*>();
	
	// initialize random seed
	srand ( time(NULL) );
	
	// Generate partition-optimized queries
	for(int i = 0; i < argc - 2; i++) {
		for(int j = 0; j < counts[i]; j++) {
			std::stringstream * text = new std::stringstream();
			genQuery(*text, i);
			queries->push_back(text);	
		}
	}
	
	// Generate non optimized queries
	for(int i = 0; i < totalNotOptQueries; i++) {
		int queryNum = rand() % 4 + 15;	// Non-optimized queries between 15 & 18
		std::stringstream * text = new std::stringstream();
		genQuery(*text, queryNum);
		queries->push_back(text);
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
