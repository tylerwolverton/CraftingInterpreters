#include "value.h"
#include "memory.h"

#include <stdio.h>

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
    printf("%g", value);
}