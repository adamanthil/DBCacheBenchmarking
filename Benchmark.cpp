#ifndef BENCHMARK_H_
#define BENCHMARK_H_

// -------------------------------------------------------
// Creates threads to execute queries on standard input 
// -------------------------------------------------------

#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string>
#include <queue>

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

// Declare global mutex for thread safety to make multi-threading easier
pthread_mutex_t mutexqueue;

class QueryContainer {
public:
	int queryNum;
	std::string * query;
};


void * executeQueries(void * ptr)
{
	std::queue<QueryContainer*> * queue;
	queue = (std::queue<QueryContainer*> *) ptr;

	QueryContainer * container;
	Parser p;

	pthread_mutex_lock (&mutexqueue);
	bool empty = queue->empty();
	if(!empty) {
		container = queue->front();
		queue->pop();
	}
	pthread_mutex_unlock (&mutexqueue);

	while(!empty) {

		// Execute Query (only for testing)
		//IRelationalOperator * op =  p.parse(*container->query);
		//op->dump(std::cout);
		//delete op;
		
		// Profile Query
		Query q(0, p.parse(*container->query));
		q.profile();

		// Check if queue has more elements
		pthread_mutex_lock (&mutexqueue);
		empty = queue->empty();
		if(!empty) {
			// Free memory before resetting it
			delete container->query;
			delete container;
			
			container = queue->front();
			queue->pop();
		}
		pthread_mutex_unlock (&mutexqueue);
	}

	pthread_exit(0);
}

pthread_t ** initializeThreads(int numThreads, std::queue<QueryContainer*> * queries) {
	pthread_t ** threads = new pthread_t*[numThreads];	// Array of pointers to threads
	for(int i = 0; i < numThreads; i++) {
		threads[i] = new pthread_t();
		int retVal = pthread_create( threads[i], NULL, executeQueries, (void*) queries);
	}
	return threads;
}

void destructThreads(pthread_t ** threads, int numThreads) {
	for(int i = 0; i < numThreads; i++) {
		delete threads[i];
	}
	delete[] threads;
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
	int numThreads = atoi(argv[2]); // number of available threads
	int availThreads = numThreads;	// number of available threads
	
	// Setup database
	//DataCreator::CreateDB("createdb",false);	// Should not create every time, just load existing files
    const char * catalog = "db.xml";
    const char * files = "config";
	initialize(catalog, files);
	
	// Set materialization according to cmd line argument
	Settings::get("partition-materialization", materialization);
	
	Database * db = Database::getInstance();
	std::queue<QueryContainer*> * queries = new std::queue<QueryContainer*>();
	
	// Read standard input
	std::string * query = new std::string();	// Declare on heap for thread access.  Thread will delete it
	getline(std::cin, *query);
	
	// Loop to load all queries until there are no more to execute
	int queryNum = 0;
	while(*query != "end" && *query != "execute") {
		QueryContainer * container = new QueryContainer();	// Declared on heap, destroyed after execution
		container->queryNum = queryNum;
		container->query = query;
		
		queries->push(container);
		
		// Load next query from standard input
		query = new std::string();
		getline(std::cin, *query);
		queryNum++;
	}
	
	std::cout << "Begining Execution of " << queryNum << " queries..." << std::endl;
	
	// Setup and start threads
	pthread_mutex_init(&mutexqueue, NULL);
	pthread_t ** threads = initializeThreads(numThreads, queries);
	
	// Wait for all threads to finish
	for(int i = 0; i < numThreads; i++) {
		pthread_join( *threads[i], NULL);
	}
	
	std::cout << "Done!" << std::endl;
	
	// memory cleanup
	pthread_mutex_destroy(&mutexqueue);
	destructThreads(threads, numThreads);
	delete queries;
	
	return 0;
}

#endif