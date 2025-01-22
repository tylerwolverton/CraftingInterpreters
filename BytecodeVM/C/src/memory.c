#include "memory.h"

#include <stdlib.h>

//-------------------------------------------------------------------------------
void* reallocate(void* pointer, size_t oldSize, size_t newSize)
{
    // Free memory
    if(newSize == 0)
    {
        free(pointer);
        return NULL;
    }

    // Realloc is equivalent to malloc when oldSize is 0
    void* result = realloc(pointer, newSize);
    if (result == NULL) exit(1);
    return result;
}
