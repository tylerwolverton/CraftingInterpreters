#include "value.h"
#include "memory.h"
#include "object.h"

#include <stdio.h>
#include <string.h>

int ARRAY_CAPACITY_INITIAL_SIZE = 8;
int ARRAY_CAPACITY_GROWTH_FACTOR = 2;

//-------------------------------------------------------------------------------
// Static local functions
static int growCapacity(int oldCapacity)
{
    if(oldCapacity < ARRAY_CAPACITY_INITIAL_SIZE)
    {
        return ARRAY_CAPACITY_INITIAL_SIZE;
    }

    return oldCapacity * ARRAY_CAPACITY_GROWTH_FACTOR;
}

//-------------------------------------------------------------------------------
void initValueArray(ValueArray* array)
{
    array->count = 0;
    array->capacity = 0;
    array->values = NULL;
}

void resetValueArray(ValueArray* array)
{
    FREE_ARRAY(Value, array->values, array->capacity);
    initValueArray(array);
}

void appendToValueArray(ValueArray* array, Value value)
{
    if(array->count == array->capacity)
    {
        int oldCapacity = array->capacity;
        array->capacity = growCapacity(array->capacity);
        array->values = GROW_ARRAY(Value, array->values, oldCapacity, array->capacity * ARRAY_CAPACITY_GROWTH_FACTOR);
    }

    // Write value and increment count
    array->values[array->count++] = value;
}

void printValue(Value value) 
{
    switch (value.type) 
    {
        case VAL_BOOL:
            printf(AS_BOOL(value) ? "true" : "false");
            break;
        case VAL_NIL: printf("nil"); break;
        case VAL_NUMBER: printf("%g", AS_NUMBER(value)); break;
        case VAL_OBJ: printObject(value); break;
    }
}

bool valuesEqual(Value a, Value b) 
{
    if (a.type != b.type) return false;
    
    switch (a.type) 
    {
        case VAL_BOOL:   return AS_BOOL(a) == AS_BOOL(b);
        case VAL_NIL:    return true;
        case VAL_NUMBER: return AS_NUMBER(a) == AS_NUMBER(b);
        case VAL_OBJ:    return AS_OBJ(a) == AS_OBJ(b);
        default:         return false; // Unreachable.
    }
}