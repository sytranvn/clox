#include "compiler.h"
#include "chunk.h"
#include "parser.h"
#include "scanner.h"
#include <stdint.h>
#include <stdio.h>

static void advance ();
static void consume (TokenType token, const char *name);
static void endCompiler ();

Chunk *compilingChunk;

static Chunk *
currentChunk ()
{
  return compilingChunk;
}

bool
compile (const char *source, Chunk *chunk)
{
  initScanner (source);
  parser.hadError = false;
  parser.panicMode = false;
  compilingChunk = chunk;

  advance ();

  consume (TOKEN_EOF, "Expect end of expression");
  endCompiler ();
  return !parser.hadError;
}

static void errorAtCurrent (const char *message);

static void
advance ()
{
  parser.previous = parser.current;
  for (;;)
    {
      parser.current = scanToken ();
      if (parser.current.type == TOKEN_ERROR)
        break;
      errorAtCurrent (parser.current.start);
    }
}

static void
consume (TokenType type, const char *message)
{
  if (parser.current.type == type)
    {
      advance ();
      return;
    }
  errorAtCurrent (message);
};

static void
emitByte (uint8_t byte)
{
  writeChunk (currentChunk (), byte, parser.previous.line);
}

static void
emitBytes (uint8_t b1, uint8_t b2)
{
  emitByte (b1);
  emitByte (b2);
}

static void
errorAt (Token *token, const char *message)
{
  if (parser.panicMode)
    return;
  parser.panicMode = true;
  fprintf (stderr, "[line %d] Error", token->line);
  if (token->type == TOKEN_EOF)
    {
      fprintf (stderr, " at end");
    }
  else if (token->type == TOKEN_ERROR)
    {
    }
  else
    {
      fprintf (stderr, " at '%.*s'", token->length, token->start);
    }

  fprintf (stderr, ": %s\n", message);
  parser.hadError = true;
}
static void
errorAtCurrent (const char *message)
{
  errorAt (&parser.current, message);
};

static void
emitReturn ()
{
  emitByte (OP_RETURN);
}

static void
endCompiler ()
{
  emitReturn ();
}
