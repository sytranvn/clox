#include "clox/chunk.h"
#include "clox/debug.h"
#include "clox/vm.h"
#include <stdio.h>
int
main ()
{
  initVM ();
  Chunk chunk;
  initChunk (&chunk);

  int constant = addConstant (&chunk, 1.2);
  writeChunk (&chunk, OP_CONST, 12);
  writeChunk (&chunk, constant, 12);

  writeChunk (&chunk, OP_RETURN, 12);
  // disassemChunk (&chunk, "test");

  interpret (&chunk);
  freeVM ();
  freeChunk (&chunk);
  return 0;
}
