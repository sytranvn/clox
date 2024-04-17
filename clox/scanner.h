#ifndef clox_scanner_h
#define clox_scanner_h

#include "vm.h"
typedef enum
{
  // clang-format off
  // one char token
  TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
  TOKEN_LEFT_BRACKET, TOKEN_RIGHT_BRACKET,
  TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
  TOKEN_COMMA, TOKEN_DOT, TOKEN_SEMICOL, 
  TOKEN_PLUS, TOKEN_MINUS, TOKEN_SLASH, TOKEN_STAR, 

  // one or two chars token
  TOKEN_BANG, TOKEN_BANG_EQ,
  TOKEN_EQ, TOKEN_EQ_EQ,  
  // TODO: TOKEN_EQ_EQ_EQ?
  TOKEN_LESS, TOKEN_LESS_EQ, TOKEN_LESS_LESS,
  TOKEN_GREATER, TOKEN_GREATER_EQ, TOKEN_GREATER_GREATER, 

  // 
  TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_STRING,

  // keywords
  TOKEN_IN, TOKEN_FOR, TOKEN_WHILE,
  TOKEN_IF, TOKEN_ELIF, TOKEN_ELSE,
  TOKEN_AND, TOKEN_OR,
  TOKEN_TRUE, TOKEN_FALSE,
  TOKEN_FUNC, TOKEN_VAR,

  TOKEN_ERROR, TOKEN_EOF
  // clang-format on
} TokenType;

typedef struct
{
  TokenType type;
  const char *start;
  int length;
  int line;
} Token;

void initScanner (const char *source);
Token scanToken ();

Token makeErrorToken ();
Token makeToken (TokenType tokenType);
#endif
