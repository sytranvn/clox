#ifndef clox_parser_h
#define clox_parser_h

#include "scanner.h"
typedef struct
{
  Token previous;
  Token current;
  bool hadError;
  bool panicMode;
} Parser;

Parser parser;
#endif
