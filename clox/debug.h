#ifndef clox_debug_h
#define clox_debug_h

#include "chunk.h"
#include "value.h"

void disassemChunk (Chunk *chunk, const char *name);
int disassemInstruction (Chunk *chunk, int offset);
int simpleInstruction (const char *name, int offset);
int constInstruction (const char *name, Chunk *chunk, int offset);
void printValue (Value val);

#endif
