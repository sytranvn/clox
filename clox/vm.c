#include "vm.h"
#include "chunk.h"
#include "common.h"
#include "debug.h"
#include "value.h"
#include <stdio.h>

// TODO: allow multiple VMs run at the same time like v8
VM vm;

static void
resetStack ()
{
  vm.stackTop = vm.stack;
}

void
initVM ()
{
  resetStack ();
};

void freeVM (){};

void
push (Value val)
{
  *vm.stackTop = val;
  vm.stackTop++;
}

Value
pop ()
{
  vm.stackTop--;
  return *vm.stackTop;
}

InterpretResult
interpret (Chunk *chunk)
{
  vm.chunk = chunk;
  vm.ip = vm.chunk->code;
  return run ();
}

InterpretResult
run ()
{
  // TODO: for performance look up “direct threaded code”, “jump table”, and
  // “computed goto”.
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE ()])

  for (;;)
    {

#ifdef DEBUG_TRACE_EXECUTION

      printf ("          ");
      for (Value *slot = vm.stack; slot != vm.stackTop; slot++)
        {
          printf ("[ ");
          printValue (*slot);
          printf (" ]");
        }
      printf ("\n");
      disassemInstruction (vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif

      uint8_t instruction;
      switch (instruction = READ_BYTE ())
        {
        case OP_CONST:
          {
            Value constant = READ_CONSTANT ();
            push (constant);
            break;
          }
        case OP_RETURN:
          printValue (pop ());
          printf ("\n");
          return INTERPRET_OK;
        }
    }

#undef READ_CONSTANT
#undef READ_BYTE
}
