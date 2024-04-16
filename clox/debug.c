#include "debug.h"
#include <stdio.h>

void
disassemChunk (Chunk *chunk, const char *name)
{
  printf ("== %s ==\n", name);
  for (int offset = 0; offset < chunk->count;)
    {
      offset = disassemInstruction (chunk, offset);
    }
}

int
disassemInstruction (Chunk *chunk, int offset)
{
  printf ("%04d ", offset);
  if (offset > 0 && chunk->lines[offset - 1] == chunk->lines[offset])
    {
      printf ("   | ");
    }
  else
    {
      printf ("%4d ", chunk->lines[offset]);
    }
  uint8_t instruction = chunk->code[offset];
  switch (instruction)
    {
    case OP_RETURN:
      return simpleInstruction ("OP_RETURN", offset);
    case OP_CONST:
      return constInstruction ("OP_CONST", chunk, offset);
    default:
      printf ("Unknown opcode %d\n", instruction);
      return offset + 1;
    }
}

int
simpleInstruction (const char *name, int offset)
{
  printf ("%s\n", name);
  return offset + 1;
}

int
constInstruction (const char *name, Chunk *chunk, int offset)
{
  uint8_t constant = chunk->code[offset + 1];
  printf ("%-16s %4d '", name, constant);
  printValue (chunk->constants.values[constant]);
  printf ("'\n");
  return offset + 2;
}

void
printValue (Value val)
{
  printf ("%g", val);
}
