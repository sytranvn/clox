#include "relp.h"
#include "vm.h"
#include <stdio.h>
#include <string.h>

void
relp ()
{

  char line[256];
  for (;;)
    {
      printf ("> ");
      if (!fgets (line, 256, stdin))
        {
          printf ("\n");
        }

      if (strcmp (line, "exit\n") == 0)
        {
          break;
        }
      interpret (line);
    }
}
