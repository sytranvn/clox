#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include <stdint.h>

typedef struct
{
  Chunk *chunk;
  uint8_t *ip;
} VM;

typedef enum
{
  INTERPRET_OK,
  INTERPRET_COMPILE_ERR,
  INTERPRET_RUNTIME_ERR,
} InterpretResult;

void initVM ();
void freeVM ();
InterpretResult interpret (Chunk *chunk);

#endif
