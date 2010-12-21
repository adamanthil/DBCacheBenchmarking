#ifndef BENCHMARK_H_
#define BENCHMARK_H_

// -------------------------------------------------------
// Creates threads to execute queries on standard input 
// -------------------------------------------------------

#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

#include "Parser.h"
#include "Settings.h"

#include "Database.h"
#include "FileManager.h"
#include "BufferManager.h"
#include "Query.h"

#include "Schema.h"
#include "Attribute.h"
#include "Operators.h"

#include "Operand.h"
#include "BooleanExpression.h"
#include "DataCreator.h"

using namespace lexer;

void profile(const std::string & query)
{
  Parser p;
  Query q(0, p.parse(query));
  q.profile();
}

void initialize(const char * catalog = "db.xml", const char * files = "config")
{
  std::cout << "initializing..."; std::cout.flush();
  BufferManager::Initialize();
  std::cout << "loading database info..."; std::cout.flush();
  Database::Initialize(catalog);
  std::cout << "loading database tables..."; std::cout.flush();
  FileManager::Initialize(files);
  std::cout << "done" << std::endl;
};

// entry point
// arg1: enable partition-materialization (1 or 0)
// arg2: number of threads to initialize
//
int main(int argc, char ** argv)
{
	// check arguments
	if(argc < 3) {
		std::cout << "Must supply 2 arguments: [partition-materialization] [number of threads]" << std::endl;
		exit(1);
	}
	
	bool materialization = atoi(argv[1]);	// use materialization
	int numThreads = atoi(argv[2]); // number of available threads
	int availThreads = numThreads;	// number of available threads
	
	// Setup database
	DataCreator::CreateDB("createdb",false);
    const char * catalog = "db.xml";
    const char * files = "config";
	initialize(catalog, files);
	
	// Set materialization according to cmd line argument
	Settings::get("partition-materialization", materialization);
	
	Database * db = Database::getInstance();
	
	// Setup Threads
	pthread_t * threads = new pthread_t[numThreads];
	bool * threadInUse = new bool[numThreads];
	memset(threadInUse, 0, numThreads);	// Set to 0
	
	// Read standard input
	std::string query;
	getline(std::cin, query);
	//std::cin >> query;
	int queryNum = 0;
	while(query != "end" && query != "quit" && query != "exit") {
		
		std::cout << "Executing query "<< queryNum << std::endl;
		
		profile(query);
		
		getline(std::cin, query);
		queryNum++;
	}
	
	// memory cleanup
	delete[] threads;
	
	return 0;
}

#endif