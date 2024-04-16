#include "value.h"
#include "memory.h"

void
initValueArray (ValueArray *array)
{
  array->count = 0;
  array->capacity = 0;
  array->values = NULL;
};
void
writeValueArray (ValueArray *array, Value value)
{
  if (array->capacity < array->count + 1)
    {
      int oldCap = array->capacity;
      array->capacity = GROW_CAPACITY (oldCap);
      array->values
          = GROW_ARRAY (Value, array->values, oldCap, array->capacity);
    }
  array->values[array->count] = value;
  array->count++;
};
void
freeValueArray (ValueArray *array)
{
  FREE_ARRAY (Value, array->values, array->capacity);
  initValueArray (array);
};
