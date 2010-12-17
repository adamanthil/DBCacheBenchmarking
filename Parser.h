#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <vector>

#include "Scanner.h"

#include "Column.h"
#include "Clause.h"
#include "IRelationalOperator.h"
#include "Operators.h"
#include "Operand.h"
#include "Schema.h"
#include "BooleanExpression.h"

typedef ConstantOperand<int> IntConstant;
typedef VariableOperand<int> IntVariable;

typedef ConstantOperand<char> CharConstant;
typedef VariableOperand<char> CharVariable;

typedef ConstantOperand<const char *> StringConstant;
typedef VariableOperand<const char *> StringVariable;

#define INT_CONSTANT(a) (Operand<int> *)new IntConstant(atoi(a.c_str()), INTEGER)
#define INT_VARIABLE(a) (Operand<int> *)new IntVariable((a), INTEGER, sizeof(int))
#define INT_OPERAND(a, type) type == VARIABLE ? INT_VARIABLE(a) : INT_CONSTANT(a)

#define CHAR_CONSTANT(a) (Operand<char> *)new CharConstant(a.c_str()[0], CHAR)
#define CHAR_VARIABLE(a) (Operand<char> *)new CharVariable((a), CHAR, sizeof(char))
#define CHAR_OPERAND(a, type) type == VARIABLE ? CHAR_VARIABLE(a) : CHAR_CONSTANT(a)

#define STRING_CONSTANT(a) (Operand<const char *> *)new StringConstant(a.c_str(), STRING)
#define STRING_VARIABLE(a, sz) (Operand<const char *> *)new StringVariable(a, STRING, sz)
#define STRING_OPERAND(a, type, sz) type == VARIABLE ? STRING_CONSTANT(a) : STRING_CONSTANT(a)

namespace lexer
{
  class SymbolTable
  {
  private: 
    static const Schema * m_symbols;
  public:
    static field_type_t type(const std::string & name)
    {
      return (*SymbolTable::m_symbols)[name]->type();
    }
    static void scope(const Schema * symbols)
    {
      SymbolTable::m_symbols = symbols;
    }
  };

  class Parser
  {
  private:
    Scanner m_scanner;
    token m_token;
    std::vector<std::string> m_columns;
  public:
    Parser();
    IRelationalOperator * parse(const std::string & query);
  private:

    void query();
    IRelationalOperator * query_op();

    /* relational operator */
    IRelationalOperator * relop();
    Projection * projection();
    MergeJoin * merge_join();

    /* scan operator */
    SequentialScan * scan();
    SequentialScan * sscan();
    void iscan();

    /* clause (join/where) */
    Columns * equi_join_clause();
    BooleanExpression * where_clause();
    BooleanExpression * boolean_expression();
    BooleanTerm * boolean_term();
    IBooleanFactor * boolean_factor();
    IBooleanFactor * boolean_factor(std::string & l, Type, std::string & r, Type, 
				    IBooleanFactor::LogicalOperator op, field_type_t t,
				    size_t field_len);
    bool logical_op();
    IBooleanFactor::LogicalOperator logical_op(token t);

    /* projection list */
    Columns * projection_list();

    bool match(token t);
    bool consume(token t);
  };
}

#endif
