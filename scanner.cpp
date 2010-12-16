#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <iostream>

#include "scanner.h"

using namespace lexer;

Scanner::Scanner() 
  : m_query(NULL), m_position(0), m_value("")
{
}

void Scanner::tokenize(const char * query)
{
  m_query = query;
  m_position = 0;
};

token Scanner::next()
{
  m_value = "";
  if (!m_query[m_position])
    {
      return EOQ;
    }

  /* consume white space */
  while (isblank(m_query[m_position]))
    m_position++;

  if (isdigit(m_query[m_position]))
    {
      int i = 0;
      char buffer[64];
      
      memset(buffer, 0, sizeof(buffer));
      buffer[i++] = m_query[m_position++];

      while (isdigit(m_query[m_position]))
	buffer[i++] = m_query[m_position++];
      m_value = buffer;

      return NUMBER;
    }
  
  if (isalpha(m_query[m_position]))
    {
      return identifier();
    }
  
  switch (m_query[m_position++])
    {
    case '(':
      return LPAREN;
    case ')':
      return RPAREN;
    case ';':
      return SEMI_COLON;
    case ',':
      return COMMA;
    case '=':
      return EQ;
    case '<':
      {
	if (m_query[m_position] == '=')
	  {
	    ++m_position;
	    return LE;
	  }
	else if (m_query[m_position] == '>')
	  {
	    ++m_position;
	    return NE;
	  }
	return LT;
      }
    case '>':
      {
	if (m_query[m_position] == '=')
	  {
	    ++m_position;
	    return GE;
	  }
	return GT;
      }
    case '!':
      {
	if (m_query[m_position] == '=')
	  {
	    ++m_position;
	    return NE;
	  }
      }
    case ':':
      return COLON;
    case '&':
      return AND;
    case '|':
      return OR;
    case '"':
    case '\'':
      {
	m_position--;
	return string_literal();
      }
    default:
      return UNKNOWN;
    }
}

token Scanner::string_literal()
{
  char marker = m_query[m_position++];
  char buffer[256];

  memset(buffer, 0, sizeof(buffer));
  for (int i = 0; m_query[m_position] != marker; i++)
    buffer[i] = m_query[m_position++];
  m_value = buffer;

  m_position++; // pop end-of-string marker. 
  return STRING_LITERAL;
}

token Scanner::identifier()
{
  char buffer[256];
  int i = 0;

  memset(buffer, 0, sizeof(buffer));
  buffer[i++] = m_query[m_position++];

  while (isalnum(m_query[m_position]))
    {
      buffer[i++] = m_query[m_position++];
    }

  /* determine if qualified attribute. */
  if (m_query[m_position] == '.')
    {
      buffer[i++] = m_query[m_position++];
      while (isalnum(m_query[m_position]))
	buffer[i++] = m_query[m_position++];

      m_value = buffer;
      return ATTRIBUTE;
    }

  /* determine if relational operator. */
  if (m_query[m_position] == '-')
    {
      buffer[i++] = m_query[m_position++];
      while (isalpha(m_query[m_position]))
	buffer[i++] = m_query[m_position++];
	
      m_value = buffer;

      if (strcasecmp(buffer, "merge-join") == 0)
	return SORT_MERGE_JOIN;
      else if (strcasecmp(buffer, "nested-join") == 0)
	return NESTED_BLOCK_JOIN;
    }

  m_value = buffer;

  /* determine if relational operator. */
  if (strcasecmp(buffer, "projection") == 0)
    return PROJECTION;
  else if (strcasecmp(buffer, "selection") == 0)
    return SELECTION;

  /* determine if scan operator */
  if (strcasecmp(buffer, "sscan") == 0)
    return SEQUENTIAL_SCAN;
  else if (strcasecmp(buffer, "iscan") == 0)
    return INDEX_SCAN;

  return IDENTIFIER;
}

const char * Scanner::description(token t)
{
  switch (t)
    {
    case PROJECTION:
      return "projection";
    case SELECTION:
      return "selection";
    case NESTED_BLOCK_JOIN:
      return "njoin";
    case SORT_MERGE_JOIN:
      return "mjoin";
      
    case SEQUENTIAL_SCAN:
      return "sscan";
    
    case LPAREN:
      return ")";
    case RPAREN:
      return ")";
    case SEMI_COLON:
      return ";";
    case COMMA:
      return ",";

    case TABLE:
      return "<tabe>";
    case ATTRIBUTE:
      return "<field>";
    case NUMBER:
      return "number";
    case IDENTIFIER:
      return "<identifier>";

    case EQ:
      return "=";
    case LT:
      return "<";
    case LE:
      return "<=";
    case GT:
      return ">=";
    case GE:
      return ">";
    case NE:
      return "!=";

    case OR:
      return "|";
    case AND:
      return "&";

    case EOQ:
      return "end-of-query";
    case UNKNOWN:
    default:
      return "?";
    }
}
