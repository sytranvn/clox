#include "scanner.h"
#include <string.h>

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

static bool
isAtEnd ()
{
  return *scanner.current == '\0';
}

static char
advance ()
{
  scanner.current++;
  return scanner.current[-1];
}

static char
peek ()
{
  return scanner.current[0];
}

static char
peekNext ()
{
  return scanner.current[1];
}

static void
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

static bool
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

static Token
string ()
{
  while (peek () != '"' && !isAtEnd ())
    {
      if (peek () == '\n')
        scanner.line++;
      advance ();
    }
  if (isAtEnd ())
    return errorToken ("Unterminated string.");
  advance (); // closing "
  return makeToken (TOKEN_STRING);
}

bool
isDigit (char c)
{
  return c >= '0' && c <= '9';
}

static Token
number ()
{
  if (peek () == '0')
    advance ();

  char c = peek ();
  bool based = false;
  if (c == 'b' || c == 'o' || c == 'x')
    {
      if (isDigit (peekNext ()))
        {
          advance ();
          based = true;
        }
      else
        return errorToken ("Invalid or unexpected token");
    }

  while (isDigit (peek ()))
    advance ();
  if (peek () == '.' && based)
    return errorToken ("Unexpected number");
  if (peek () == '.' && isDigit (peekNext ()))
    advance ();
  while (isDigit (peek ()))
    {
      advance ();
    }
  return makeToken (TOKEN_NUMBER);
}

bool
isAlpha (char c)
{
  return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_';
}

static TokenType
checkKeyword (int start, int length, const char *rest, TokenType type)
{
  if (scanner.current - scanner.start == start + length
      && memcmp (scanner.start + start, rest, length) == 0)
    return type;
  return TOKEN_IDENTIFIER;
}

static TokenType
identifierType ()
{
  switch (scanner.current[0])
    {

    case 'a':
      return checkKeyword (1, 2, "nd", TOKEN_AND);
    case 'i':
      {
        if (scanner.current - scanner.start > 1)
          {
            switch (scanner.start[1])
              {
              case 'f':
                return checkKeyword (1, 2, "f", TOKEN_IF);
              case 'n':
                return checkKeyword (1, 2, "n", TOKEN_IN);
              }
          }
      }
    case 't':
      return checkKeyword (1, 2, "rue", TOKEN_TRUE);
    case 'e':
      {
        if (scanner.current - scanner.start > 2)
          {
            switch (scanner.start[2])
              {
              case 'i':
                return checkKeyword (1, 3, "lif", TOKEN_ELIF);
              case 's':
                return checkKeyword (1, 2, "lse", TOKEN_ELSE);
              }
          }
      }
    case 'f':
      {

        if (scanner.current - scanner.start > 2)
          {
            switch (scanner.start[1])
              {
              case 'u':
                return checkKeyword (1, 3, "unc", TOKEN_FUNC);
              case 'a':
                return checkKeyword (1, 4, "alse", TOKEN_FALSE);
              case 'o':
                return checkKeyword (1, 2, "or", TOKEN_FOR);
              }
          }
      }
    case 'o':
      return checkKeyword (1, 2, "r", TOKEN_OR);
    case 'v':
      return checkKeyword (1, 2, "ar", TOKEN_VAR);
    case 'w':
      return checkKeyword (1, 2, "hile", TOKEN_WHILE);
    }
  return TOKEN_IDENTIFIER;
}

static Token
identifier ()
{
  while (isAlpha (peek ()) || isDigit (peek ()))
    advance ();
  return makeToken (identifierType ());
}

Token
scanToken ()
{
  scanner.start = scanner.current;
  skipWhiteSpace ();
  if (isAtEnd ())
    return makeToken (TOKEN_EOF);

  char c = advance ();

  if (isAlpha (c))
    return identifier ();
  if (isDigit (c))
    return number ();
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
      if (peekNext () == '/')
        {
          while (peek () != '\n' && !isAtEnd ())
            {
              advance ();
            }
        }
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

  return errorToken ("Unexpected character.");
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
errorToken (const char *name)
{
  Token token;
  token.type = TOKEN_ERROR;
  token.start = scanner.current;
  token.line = scanner.line;
  token.length = scanner.current - scanner.start;

  return token;
}
