#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "value.h"

typedef struct
{
  Chunk *chunk;
  uint8_t *ip; // point at the current opcode
  Value stack[STACK_MAX];
  Value *stackTop; // point at the top of stack
} VM;

typedef enum
{
  INTERPRET_OK,
  INTERPRET_COMPILE_ERR,
  INTERPRET_RUNTIME_ERR,
} InterpretResult;

void initVM ();
void freeVM ();
InterpretResult interpret (const char *line);
InterpretResult run ();

void push (Value val);
Value pop ();

#endif
