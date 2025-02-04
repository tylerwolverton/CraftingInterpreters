#include "memory.h"
#include "vm.h"

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

static void freeObject(Obj* object) 
{
    switch (object->type) 
    {
        case OBJ_STRING: 
        {
            ObjString* string = (ObjString*)object;
            FREE_ARRAY(char, string->chars, string->length + 1);
            FREE(ObjString, object);
            break;
        }
    }
}

void freeObjects() 
{
    Obj* object = vm.objects;
    while (object != NULL) 
    {
        Obj* next = object->next;
        freeObject(object);
        object = next;
    }
}