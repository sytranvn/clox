#ifndef clox_chunk_h
#define clox_chunk_h

#include "value.h"

#define STACK_MAX 256

typedef enum
{
  OP_CONST,
  OP_RETURN
} OpCode;

typedef struct
{
  int count;
  int capacity;
  uint8_t *code;
  int *lines; // TODO: this waste memory, use run-length encoding to reduce
              // memory for lines
  ValueArray constants;
} Chunk;

void initChunk (Chunk *chunk);
void writeChunk (Chunk *chunk, uint8_t byte, int line);
void freeChunk (Chunk *chunk);
int addConstant (Chunk *chunk, Value value);
/**  determines the line where the instruction occurs */
int getLine (Chunk *chunk, int index);

#endif
