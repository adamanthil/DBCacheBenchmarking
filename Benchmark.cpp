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

class ThreadContainer {
public:
	int threadNum;
	std::string * query;
};

// Declare globals to make multi-threading easier
bool * threadInUse;
pthread_t ** threads;
int numThreads;

void * profile(void * ptr)
{
  ThreadContainer * container;
  container = (ThreadContainer *) ptr;
	
  Parser p;

  IRelationalOperator * op =  p.parse(*container->query);
  op->dump(std::cout);
  delete op;

  //Query q(0, p.parse(*container->query));
  //q.profile();

  // mark thread as completed
  threadInUse[container->threadNum] = false;

  // Free memory
  delete container->query;
  delete container;

  pthread_exit(0);
}

void initializeThreads() {
	threads = new pthread_t*[numThreads];	// Array of pointers to threads
	for(int i = 0; i < numThreads; i++) {
		threads[i] = new pthread_t();
	}
}

void destructThreads() {
	for(int i = 0; i < numThreads; i++) {
		delete threads[i];
	}
	delete[] threads;
}

// Looks in bool array to determine the next available thread and saves offset in nextThread
bool availableThread(int & nextThread) {
	for(int i = 0; i < numThreads; i++) {
		if(!threadInUse[i]) {
			nextThread = i;
			threadInUse[i] = true;
			return true;
		}
	}
	return false;
}

// Initializes the database
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
	numThreads = atoi(argv[2]); // number of available threads
	int availThreads = numThreads;	// number of available threads
	
	// Setup database
	//DataCreator::CreateDB("createdb",false);	// Should not create every time, just load existing files
    const char * catalog = "db.xml";
    const char * files = "config";
	initialize(catalog, files);
	
	// Set materialization according to cmd line argument
	Settings::get("partition-materialization", materialization);
	
	Database * db = Database::getInstance();
	
	// Setup Threads
	initializeThreads();	// Initialize array of pointers to threads
	threadInUse = new bool[numThreads];
	memset(threadInUse, 0, numThreads);	// Set to 0
	
	// Read standard input
	std::string * query = new std::string();	// Declare on heap for thread access.  Thread will delete it
	getline(std::cin, *query);
	
	int queryNum = 0;
	pthread_t nextThread;
	int threadOffset = -1;
	
	// Loop till there are no more queries to execute
	while(*query != "end" && *query != "quit" && *query != "exit") {
		
		// Check if there is an available thread
		if(availableThread(threadOffset)) {

			std::cout << "Executing query "<< queryNum << " in thread " << threadOffset << std::endl;

			// Instantiate a new container to pass to the thread.  Thread will delete it
			ThreadContainer * container = new ThreadContainer();
			container->threadNum = threadOffset;
			container->query = query;
			
			int retVal = pthread_create( threads[threadOffset], NULL, profile, (void*) &container);
			
			//profile(container);

			query = new std::string();
			getline(std::cin, *query);
			queryNum++;
		}

	}
	
	// memory cleanup
	destructThreads();
	delete[] threadInUse;
	
	return 0;
}

#endif