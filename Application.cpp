#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <new>

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

typedef ConstantOperand<int> IntConstant;
typedef ConstantOperand<const char *> StringConstant;

typedef VariableOperand<int> IntVariable;
typedef VariableOperand<const char *> StringVariable;

void profile(const std::string & query, int  count = 1)
{
  Parser p;
  Query q(0, p.parse(query));
  q.profile(count);

  q.stats(std::cout);
}

void query(const std::string & query)
{
  Parser p;
  IRelationalOperator * op =  p.parse(query);
  
  op->dump(std::cout);
  delete op;
}

void usage()
{
  std::cout << "dblite [<layout-file> <schema-file>]" << std::endl
	    << "command       options                   description" << std::endl
	    << "=======       =======                   ===========" << std::endl
	    << "help                                    display usage/help" << std::endl
	    << "query         <query>                   execute query, results are returned to stdout" << std::endl
	    << "profile       [count] <query>           profiles the query" << std::endl
	    << "layout        ?|f|p                     gets/sets the current materialization" << std::endl
	    << "              ? - get current layout" << std::endl
	    << "              f - singl partition" << std::endl
	    << "              p - 2-partitons" << std::endl
	    << "quit                                    exits the program" << std::endl
	    << std::endl;
}

void imode()
{
  std::string cmd;
  std::string history;

  Database * db = Database::getInstance();

  std::cout << "> ";
  std::cout.flush();

  std::cin >> cmd;
  while (cmd != "quit" && cmd != "exit")
    {
      if (cmd == "tables")
	{
	  const std::vector<Table *> & tables = db->tables();
	  for (int i = 0; i < tables.size(); i++)
	    {
	      std::cout << tables.at(i)->name() << std::endl;
	    }
	}
      else if (cmd == "create")
	{

	  std::string file;
	  getline(std::cin, file);
	  std::cout << "****program will exit once complete****"
		    << std::endl;
	  DataCreator::CreateDB(file.c_str() + 1, false);	
	  std::cout << "done...exiting" << std::endl;
	  exit(0); 
	}
      else if (cmd == "describe")
	{
	  std::string tbl;
	  std::cin >> tbl; 

	  if (tbl != "" && db->table(tbl))
	    {
	      const Table * table = db->table(tbl);
	      const Schema * schema = table->schema();
	      
	      std::cout << "column\ttype\t\tsize"
			<< std::endl
			<< "======\t====\t\t===="
			<< std::endl;

	      for (int i = 0; i < schema->nitems(); i++)
		{
		  const Attribute * attribute = schema->at(i);
		  std::cout << attribute->name() << "\t" << Attribute::description(attribute->type()) 
			    << "\t\t" << attribute->size() << std::endl;
		}
	    }
	  else
	    {
	      std::cout << "invalid table name. type 'tables' for list of tables" << std::endl;
	    }
	}
      else if (cmd == "query" || cmd == "profile")
	{
	  std::string q;
	  
	  if (cmd == "query")
	    {
	      getline(std::cin, q);
	      query(q);
	    }
	  else
	    {
	      std::string s;
	      int count = 0;
	      std::cin >> s;
	      if ((count = atoi(s.c_str())))
		{
		  getline(std::cin, q);
		}
	      else
		{
		  count = 1;
		  q = s;
		}
	      profile(q, count);
	    }
	}
      else if (cmd == "layout")
	{
	  std::string option;
	  bool enabled = false;
	  
	  getline(std::cin, option);

	  if (option == " ?")
	    {
	      Settings::get("partition-materialization", enabled);
	      std::cout << "layout = " << (!enabled ? "flat" : "partitioned")
			<< std::endl;
	    }
	  else if (option == " f")
	    {
	      Settings::set("partition-materialization", false);
	    }
	  else if (option == " p")
	    {
	      Settings::set("partition-materialization", true);
	    }
	  else
	    {
	      std::cerr << "Invalid option '" << option << "'; valid options are: [?, p, f]"
			<< std::endl;
	    }

	}
      else if (cmd == "help")
	{
	  std::cout << "display usage" << std::endl;
	  usage();
	}
      else
	{
	  std::cout << "unknown command" << std::endl
		    << "type help for usage" << std::endl;
	}

      std::cout << "> ";
      std::cout.flush();

      history = cmd;
      std::cin >> cmd;
    }
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

int main(int argc, char ** argv)
{
  const char * catalog = "db.xml";
  const char * files = "config";

  Settings::set("partition-materialization", true);

  if (argc >= 2)
    files = argv[1];
  if (argc >= 3)
    catalog = argv[2];
    
  
  initialize(catalog, files);
  imode();
}

#endif
