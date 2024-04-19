#include "compiler.h"
#include "scanner.h"

bool
compile (const char *source, Chunk *chunk)
{
  initScanner (source);

  advance ();

  consume (TOKEN_EOF, "Expect end of expression");
}
