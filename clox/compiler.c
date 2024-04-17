#include "compiler.h"
#include "scanner.h"

void
compile (const char *source)
{
  initScanner (source);

  for (;;)
    {
      Token token = scanToken ();
    }
}
