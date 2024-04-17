#include "scanner.h"

typedef struct
{
  const char *start;
  const char *current;

  int line;
} Scanner;

Scanner scanner;

void
initScanner (const char *source)
{
  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
}

bool
isAtEnd ()
{
  return *scanner.current == '\0';
}

char
advance ()
{
  scanner.current++;
  return scanner.current[-1];
}

char
peek ()
{
  return scanner.current[0];
}

void
skipWhiteSpace ()
{
  for (;;)
    {
      char c = peek ();
      switch (c)
        {
        case ' ':
        case '\r':
        case '\t':
          advance ();
          break;
        case '\n':
          {
            advance ();
            scanner.line++;
          }
          break;

        default:
          return;
        }
    }
}

bool
match (const char c)
{
  if (isAtEnd ())
    return false;
  if (*scanner.current != c)
    {
      return false;
    }
  scanner.current++;
  return true;
}

Token
string ()
{
  while (peek () != '"' && !isAtEnd ())
    {
      if (peek () == '\n')
        scanner.line++;
      advance ();
    }
  if (isAtEnd ())
    return makeErrorToken ();
  advance (); // closing "
  return makeToken (TOKEN_STRING);
}

Token
scanToken ()
{
  scanner.start = scanner.current;
  skipWhiteSpace ();
  if (isAtEnd ())
    return makeToken (TOKEN_EOF);
  char c = advance ();
  switch (c)
    {
    case '{':
      return makeToken (TOKEN_LEFT_BRACE);
    case '}':
      return makeToken (TOKEN_RIGHT_BRACE);
    case '[':
      return makeToken (TOKEN_LEFT_BRACKET);
    case ']':
      return makeToken (TOKEN_RIGHT_BRACKET);
    case '(':
      return makeToken (TOKEN_LEFT_PAREN);
    case ')':
      return makeToken (TOKEN_RIGHT_PAREN);

    case ',':
      return makeToken (TOKEN_COMMA);
    case '.':
      return makeToken (TOKEN_DOT);
    case ';':
      return makeToken (TOKEN_SEMICOL);
    case '+':
      return makeToken (TOKEN_PLUS);
    case '-':
      return makeToken (TOKEN_MINUS);
    case '/':
      return makeToken (TOKEN_SLASH);
    case '*':
      return makeToken (TOKEN_STAR);

    case '!':
      {

        return match ('=') ? makeToken (TOKEN_BANG_EQ)
                           : makeToken (TOKEN_BANG);
      }

    case '=':
      {
        return match ('=') ? makeToken (TOKEN_EQ_EQ) : makeToken (TOKEN_EQ);
      }

    case '<':
      {
        if (match ('='))
          return makeToken (TOKEN_LESS_EQ);
        if (match ('<'))
          return makeToken (TOKEN_LESS_LESS);

        return makeToken (TOKEN_LESS);
      }

    case '>':
      {
        if (match ('='))
          return makeToken (TOKEN_GREATER_EQ);
        if (match ('>'))
          return makeToken (TOKEN_GREATER_GREATER);

        return makeToken (TOKEN_GREATER);
      }
    case '"':
      {
        return string ();
      }
    }

  return makeErrorToken ();
}

Token
makeToken (TokenType tokenType)
{
  Token token;
  token.type = tokenType;
  token.start = scanner.current;
  token.line = scanner.line;
  token.length = scanner.current - scanner.start;

  return token;
}

Token
makeErrorToken ()
{
  Token token;
  token.type = TOKEN_ERROR;
  token.start = scanner.current;
  token.line = scanner.line;
  token.length = scanner.current - scanner.start;

  return token;
}
