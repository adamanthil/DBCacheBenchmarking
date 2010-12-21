#ifndef SCANNER_H_
#define SCANNER_H_

#include <string>

namespace lexer
{  
  enum token 
  {
    /* relational operators. */
    PROJECTION = 0,
    SELECTION,
    NESTED_BLOCK_JOIN,
    SORT_MERGE_JOIN,
    
    /* scan operators */
    SEQUENTIAL_SCAN,
    INDEX_SCAN,
    
    /* separators */
    LPAREN,
    RPAREN,
    SEMI_COLON,
    COMMA,
    COLON,

    TABLE,
    ATTRIBUTE, 
    IDENTIFIER,
    NUMBER,
    STRING_LITERAL,

    /* logical operators */
    EQ,
    LT,
    GT,
    LE,
    GE,
    NE,

    /* logical connectors */
    AND,
    OR,
  
    EOQ, // end-of-query
    UNKNOWN
  };


  class Scanner
  {
  private:
    const char * m_query;
    int m_position;
    
    token identifier();
    token string_literal();
    std::string m_value;
  public:
    Scanner();
    void tokenize(const char * query);
    token next();
    const std::string & value() { return m_value; }

    static const char * description(token);
  };

}

#endif
