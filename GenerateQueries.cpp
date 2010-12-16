#ifndef GENERATE_QUERIES_H_
#define GENERATE_QUERIES_H_

#include <iostream>
#include <stdlib.h>

// Forward declarations
void genQuery(int, int);
void query0(int);
void query1(int);

// entry point
int main(int argc, char ** argv)
{
	
	// initialize random seed
	srand ( time(NULL) );
	
	// Loop through number of arguments (number of queries to run)
	for(int i = 0; i < argc; i++) {
		int count = atoi(argv[i]);
		genQuery(i-1, count);
	}
	
	return 0;
}

void genQuery(int query, int count) {
	switch(query) {
		case 0:
			query0(count);
			break;
		case 1:
			query1(count);
			break;
	}

}

void query0(int count) {
	// Generate query0 "count" times
	for(int i = 0; i < count; i++) {
		int number = rand() % 100 + 1;
		std::cout << "sscan(T1:R; R.a, R.b; R.a < " << number << ")" << std::endl;
	}
}

void query1(int count) {
	// Generate query0 "count" times
	for(int i = 0; i < count; i++) {
		int number = rand() % 100 + 1;
		std::cout << "sscan(T1:R; R.a, R.b; R.a > " << number << ")" << std::endl;
	}
}

#endif