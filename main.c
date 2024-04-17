#include "clox/chunk.h"
#include "clox/debug.h"
#include "clox/relp.h"
#include "clox/vm.h"
#include <stdio.h>
int
main (int argc, const char **args)
{
  initVM ();
  if (argc == 1)
    {
      relp ();
    }
  else if (argc == 2)
    {
    }
  else
    {
      fprintf (stderr, "%s [file]\n", args[0]);
    }
  freeVM ();
  return 0;
}
