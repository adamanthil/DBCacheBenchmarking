#include <iostream>

#include "parser.h"
#include "syscatalog.h"

using namespace lexer;

const Schema * SymbolTable::m_symbols = NULL;

Parser::Parser() 
{
}

IRelationalOperator * Parser::parse(const std::string & query)
{
  m_scanner.tokenize(query.c_str());
  m_token = m_scanner.next();

  if (m_token != EOQ)
    {
      return query_op();
    }
}

IRelationalOperator * Parser::query_op()
{
  switch (m_token)
    {
    case PROJECTION:
    case SELECTION:
    case SORT_MERGE_JOIN:
    case NESTED_BLOCK_JOIN:
      return relop();
    case SEQUENTIAL_SCAN:
      return scan();
    default:
      std::cerr << "Unexpected token: "
		<< Scanner::description(m_token) << ". Expected scan-op | rel-op";
      break;
    }
}

IRelationalOperator * Parser::relop()
{
  if (consume(PROJECTION))
    {
      if (consume(LPAREN))
	{
	  IRelationalOperator * op = projection();
	  if (consume(RPAREN))
	    {
	      return op;
	    }

	  delete op;
	  std::cerr << "Unexpected token: '" << Scanner::description(m_token)
		    << "'; Expected: '" << Scanner::description(RPAREN) << "'";
	}
      else
	{
	  std::cerr << "Unexpected token: '" << Scanner::description(m_token)
		    << "'; Expected: '" << Scanner::description(LPAREN) << "'"; 
	}
    }
  else if (consume(SORT_MERGE_JOIN))
    {
      if (consume(LPAREN))
	{
	  IRelationalOperator * op = merge_join();
	  if (consume(RPAREN))
	    return op;

	  delete op;
	}
    }
  else if (consume(NESTED_BLOCK_JOIN))
    {
    }
  else
    {
      std::cerr << "Unexpected token. Expected projection | selection | m-join | n-join";
    }

  return NULL;
}

Projection * Parser::projection()
{
  IRelationalOperator * child = query_op();

  if (consume(SEMI_COLON))
    {
      ProjectionList * list = projection_list();
      Projection * p = new Projection(child, *list);
      delete list;
      return p;
    }
  else
    {
      delete child;
      std::cerr << "Unexpected token: '" << Scanner::description(m_token)
		<< "'; Expected: '" << Scanner::description(SEMI_COLON) << "'";
    }
}

/*
 * @syntax = merge-join(l-rel-op, r-rel-op; equi-join-clause; [projection-list])
 */
MergeJoin * Parser::merge_join()
{

  IRelationalOperator * lChild = NULL;
  IRelationalOperator * rChild = NULL;

  lChild = query_op();
    
  if (!consume(COMMA))
    {
      std::cerr << "Unexpected token: '" << Scanner::description(m_token)
		<< "'; Expected: '" << Scanner::description(COMMA) << "'";

      delete lChild;

      return NULL;
    }

  rChild = query_op();	    

  if (!consume(SEMI_COLON))
    {
      delete lChild;
      delete rChild;
     
      return NULL;
    }

  Columns * joinCols = equi_join_clause();
  if (!consume(SEMI_COLON))
    {
      std::cerr << "Unexpected token: '" << Scanner::description(m_token)
		<< "'; Expected: '" << Scanner::description(SEMI_COLON) << "'";

      delete lChild;
      delete rChild;
      delete joinCols;

      return NULL;
    }
      
  if (!match(RPAREN))
    {
      ProjectionList * list = projection_list();
      MergeJoin * join = NULL; // TODO: uncomment new MergeJoin(lChild, rChild, m_columns, *list);
      
      delete list;
      delete joinCols;

      return join;
    }
  else
    {
      MergeJoin * join = new MergeJoin(lChild, rChild, *joinCols);
      
      delete joinCols;

      return join;
    }
}

SequentialScan * Parser::scan()
{
  if (consume(SEQUENTIAL_SCAN) && consume(LPAREN))
    {
      SequentialScan * scan = sscan();
      if (consume(RPAREN))
	{
	  return scan;
	}

      delete scan;
      std::cerr << "Unexpected token: '" << Scanner::description(m_token) 
		<< "'. Expected: '" << Scanner::description(RPAREN) << "'";
    }
  else
    {
      std::cerr << "Unexpected token. Expected '('";
    }
}

/*
 * @syntax = sscan(table[:alias]; [where-clause]; projection-list)
 */

SequentialScan * Parser::sscan()
{
  if (!match(IDENTIFIER))
    {
      std::cerr << "Unexpected token. Expected '<table> : alias;'";
    }

  const Table * t = SystemCatalog::table(m_scanner.value());
  std::string alias = t->name();
  // consume table. 
  consume(m_token);
      
  if (consume(COLON))
    {
      if (match(IDENTIFIER))
	{
	  alias = m_scanner.value();
	  consume(m_token); // consume table-alias.
	}
      else
	std::cerr << "Unexpected token: '" << Scanner::description(m_token) 
		  << "'; Expected: '" << Scanner::description(IDENTIFIER) << "'";
    }
      
  if (consume(SEMI_COLON))
    {	
      Columns * list = projection_list();
      
      if (consume(SEMI_COLON))
	{
	  SequentialScan * s = new SequentialScan(t, alias, *list);
	  
	  // update the symbol table
	  SymbolTable::scope(s->schema());
	  if (!match(RPAREN))
	    {
	      
	    }
	 
	  delete list;
	  return s;
	}
      else
	{
	  delete list;
	  std::cerr << "Unexected token: '" << Scanner::description(m_token) 
		    << "'. Expected '" << Scanner::description(IDENTIFIER) << "' or '"
		    << Scanner::description(SEMI_COLON) << "'";
	}
    }
 }

Columns * Parser::projection_list()
{
  Columns * list = new Columns();
  while (match(ATTRIBUTE))
    {
      std::string column = m_scanner.value();
      int offset = column.find(".");
      std::string name = column.substr(1+offset);
      std::string table = column.substr(0, offset); 
					  
      list->add(name, table);
      consume(m_token);
      if (!consume(COMMA))
	{
	  return list;
	}
    }  

  delete list;
  std::cerr << "Unexpected token: '" << Scanner::description(m_token)
	    << "'; Expected '" << Scanner::description(ATTRIBUTE) << "'";
}

BooleanExpression * Parser::where_clause()
{
  return boolean_expression();
}

Columns * Parser::equi_join_clause()
{
  
  Columns * columns = new Columns();
  do
    {
      if (!match(ATTRIBUTE))
	{
	  std::cerr << "Unpexted token: '" << Scanner::description(m_token) 
		    << "'; Expected: '" << Scanner::description(ATTRIBUTE) << "'";
	  
	  delete columns;
	  return NULL;
	}

      if (!columns->contains(m_scanner.value()))
	{
	  std::string column = m_scanner.value();
	  int offset = column.find(".");
	  std::string name = column.substr(1+offset);
	  std::string table = column.substr(0, offset); 
	  
	  columns->add(name, table);
	}
      
      consume(m_token);
      if (!consume(EQ))
	{
	  std::cerr << "Unpexted token: '" << Scanner::description(m_token) 
		    << "'; Expected: '" << Scanner::description(EQ) << "'";
	  
	  delete columns;
	  return NULL;

	}

      if (!match(ATTRIBUTE))
	{
	  std::cerr << "Unpexted token: '" << Scanner::description(m_token) 
		    << "'; Expected: '" << Scanner::description(ATTRIBUTE) << "'";
	  
	  delete columns;
	  return NULL;
	}

      if (!columns->contains(m_scanner.value()))
	{
	  std::string column = m_scanner.value();
	  int offset = column.find(".");
	  std::string name = column.substr(1+offset);
	  std::string table = column.substr(0, offset); 
	  
	  columns->add(name, table);
	}

      consume(m_token);
    }
  while (consume(AND));

  return columns;
}

BooleanExpression * Parser::boolean_expression()
{
  std::vector<BooleanTerm *> terms;
  
  m_columns.clear();

  terms.push_back(boolean_term());
  while (consume(OR))
    {
      terms.push_back(boolean_term());
    }

  BooleanExpression * e = new BooleanExpression(terms.size());
  for (int i = 0; i < terms.size(); i++)
    e->term(i, terms[i]);

  return e;
}

BooleanTerm * Parser::boolean_term()
{
  BooleanTerm * term = new BooleanTerm();
  term->factor(boolean_factor());
  while (consume(AND))
    {
      term->factor(boolean_factor());
    }

  return term;
}

IBooleanFactor * Parser::boolean_factor()
{
  std::string l;
  std::string r;

  if (match(ATTRIBUTE))
    {
      l = m_scanner.value();
      consume(m_token);
     
      if (!logical_op())
	{
	  return NULL;
	}
	  
      IBooleanFactor::LogicalOperator op = logical_op(m_token);
      consume(m_token); // consume logical operator;

      token t = m_token;
      std::string r = m_scanner.value();
      if (consume(ATTRIBUTE) || consume(NUMBER) || consume(STRING_LITERAL))
	{
	  field_type_t type = SymbolTable::type(l);
	  size_t len = 0; //

	  m_columns.push_back(l);
	  consume(t);

	  if (t == ATTRIBUTE) 
	    {
	      m_columns.push_back(r);
	    }

	  return boolean_factor(l, VARIABLE, r, t == ATTRIBUTE ? VARIABLE : CONSTANT, 
				op, type, len); 
	}
      else
	{
	  std::cerr << "Unexpected token: '" << Scanner::description(m_token)
		    << "'; Expected: '" << Scanner::description(ATTRIBUTE) 
		    << "' or '" << Scanner::description(NUMBER) << "'";
	}
    }
  else if (match(NUMBER) || match(STRING_LITERAL))
    {
      field_type_t type = match(NUMBER) ? INTEGER : STRING;
      size_t len = 0;

      consume(m_token);
      if (!logical_op()) 
	{
	  return NULL;
	}

      IBooleanFactor::LogicalOperator op = logical_op(m_token);
      consume(m_token);

      token t = m_token;
      std::string r = m_scanner.value();
      if (consume(ATTRIBUTE))
	{
	  m_columns.push_back(r);
	  return boolean_factor(l, CONSTANT, r, VARIABLE, op, type, len);
	}
      else
	{
	  std::cerr << "Unexpected token: '" << Scanner::description(m_token)
		    << "'; Expected: '" << Scanner::description(ATTRIBUTE) << "'";
	}
    }
  else
    {
      std::cerr << "Unexpected token: '" << Scanner::description(m_token)
		<< "'; Expected: '" << Scanner::description(ATTRIBUTE) << "'";
    }

  return NULL;
}

IBooleanFactor * Parser::boolean_factor(std::string & l, Type lType, 
					std::string & r, Type rType, 
					IBooleanFactor::LogicalOperator op, 
					field_type_t value_type, size_t field_len)
{ 
  if (value_type == INTEGER)
    {
      Operand<int> * lOperand = INT_OPERAND(l, lType);
      Operand<int> * rOperand = INT_OPERAND(r, rType);
      
      return new BooleanFactor<int>(lOperand, op, rOperand); 
    } 

  if (value_type == CHAR)
    {
      Operand<char> * lOperand = CHAR_OPERAND(l, lType);
      Operand<char> * rOperand = CHAR_OPERAND(r, rType);
      
      return new BooleanFactor<char>(lOperand, op, rOperand); 
    } 

  if (value_type == STRING)
    {
      Operand<const char *> * lOperand = new StringVariable(l, STRING, field_len);
      Operand<const char *> * rOperand = STRING_OPERAND(r, rType, field_len); 
      
      return new BooleanFactor<const char *>(lOperand, op, rOperand);
    }

  return NULL;
}

bool Parser::logical_op()
{
  switch (m_token)
    {
    case EQ:
    case NE:
    case LT:
    case LE:
    case GT:
    case GE:
      return true;
    default:
      std::cerr << "Unexpected token: '" << Scanner::description(m_token)
		<< "'; Expected: '" 
		<< Scanner::description(EQ) << "' or '"
		<< Scanner::description(NE) << "' or '"
		<< Scanner::description(LT) << "' or '"
		<< Scanner::description(LE) << "' or '"
		<< Scanner::description(GT) << "' or '"
		<< Scanner::description(GE) << "'";
      return false;
    }
}

IBooleanFactor::LogicalOperator Parser::logical_op(token t)
{
  switch (t)
    {
    case NE:
      return IBooleanFactor::NE;
    case LT:
      return IBooleanFactor::LT;
    case LE:
      return IBooleanFactor::LE;
    case GT:
      return IBooleanFactor::GT;
    case GE:
      return IBooleanFactor::GE;
    case EQ:
    default:
      return IBooleanFactor::EQ;
    }
}

bool Parser::match(token t)
{
  return m_token == t;
}

bool Parser::consume(token t)
{
  if (match(t))
    {
      m_token = m_scanner.next();
      return true;
    }

  return false;
}


