#ifndef SRC_LAMSCRIPT_PARSER_H_
#define SRC_LAMSCRIPT_PARSER_H_

#include <initializer_list>
#include <stdexcept>
#include <typeinfo>
#include <vector>

#include <Lamscript/Expression.h>
#include <Lamscript/Statement.h>
#include <Lamscript/Token.h>
#include <Lamscript/TokenType.h>

namespace lamscript {

/// @brief Custom Parse error for when the parser encounters an invalid
/// expression/statement.
class ParseError : std::runtime_error {
 public:
  explicit ParseError(const char* what_arg) : std::runtime_error(what_arg){}
};

/// @brief The Lamscript LL parser for converting Tokens into
/// Statements and Expressions. This evaluates Tokens presented by the scanner
/// and converts it into code that can be executed by the interpreter.
class Parser {
 public:
  explicit Parser(const std::vector<Token>& tokens)
      : tokens_(tokens), current_token_(0) {}

  /// @brief Begins parsing all tokens provided to the Parser.
  std::vector<Statement*> Parse();

 private:
  std::vector<Token> tokens_;
  int current_token_;

  /// @brief Peek at the next token that we're going to parse.
  Token Peek();

  /// @brief Look at the previously parsed token.
  Token Previous();

  /// @brief Check to see if the end of the file has been reached.
  bool HasReachedEOF();

  /// @brief Advance to the next token if it exists, otherwise give the last
  /// that was parsed.
  Token Advance();

  /// @brief Checks if the current token matches the given token type.
  bool CheckToken(const TokenType& token_type);

  /// @brief validates that the current token matches any given set of types.
  bool CheckTokens(const std::initializer_list<TokenType>& token_types);

  /// @brief Upon an error occurring, we synchronize the parser to get to the
  /// next potentially valid expression/statement.
  void Synchronize();

  /// @brief Returns an error that propagates up through the stack for hh
  ParseError Error(Token token, const std::string& message);

  /// @brief Consumes a token if it matches the type of token being passed in.
  /// throws an error if the token doesn't match.
  Token Consume(TokenType type, const std::string& message);

  // ----------------------------- PARSE STATEMENTS ----------------------------

  /// @brief Attempts to parse a variable declaration statement. If a variable
  /// declaration isn't found, it will continue trying to parse other types of
  /// statements.
  Statement* ParseDeclaration();

  /// @brief Parse Variable declaration and returns a VariableStatement when
  /// successfully done.
  Statement* ParseVariableDeclaration();

  /// @brief Parses block statements produced from bracket {} tokens and returns
  /// the list of statements that belong to that scope.
  std::vector<Statement*> ParseBlockStatements();

  /// @brief Generic statement parsing that parses for more generic statements.
  /// See the docs or cpp file for more information on precedence/order.
  Statement* ParseStatement();

  /// @brief Parses the current print and the expression that is being printed.
  Statement* ParsePrintStatement();

  /// @brief Parses the current expression statement. This is the simplest type
  /// of statement that can be produced.
  Statement* ParseExpressionStatement();

  /// @brief Parses conditional if/then/else statements.
  Statement* ParseIfStatement();

  // ---------------------------- PARSE EXPRESSIONS ----------------------------

  /// @brief Parses an equality for as long as there are equal signs and
  /// continually chain the previous expression e.g. `true == false;`
  Expression* ParseEquality();

  /// @brief Parses an assignment expression e.g. `x = 4;`
  Expression* ParseAssignment();

  /// @brief Generically parses an expression. Starts off with assignment.
  Expression* ParseExpression();

  /// @brief Parses primary expressions into literals. e.g. `"hello, world!"`
  Expression* ParsePrimary();

  /// @brief Parse Unary Tokens. e.g. `-x || !x;`
  Expression* ParseUnary();

  /// @brief Parse subtraction and addition terms. e.g. `x + 5;`
  Expression* ParseTerm();

  /// @brief Parses division and multiplication. e.g. `x * 5;`
  Expression* ParseFactor();

  /// @brief Parse the current comparison being made.
  Expression* ParseComparison();

  /// @brief Parse Or comparisons. e.g. `10 or nil;`.
  Expression* ParseOr();

  /// @brief Parse And comparisons. e.g. `10 and nil;`.
  Expression* ParseAnd();
};

}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_PARSER_H_
