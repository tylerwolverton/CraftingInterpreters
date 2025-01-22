#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "value.h"

#define STACK_MAX 256

// Save ip and stackTop as pointers as they're faster than
// indexing into an array
typedef struct {
    Chunk* chunk;
    uint8_t* ip;
    Value stack[STACK_MAX];
    Value* stackTop;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM();
void resetVM();

InterpretResult interpretChunk(Chunk* chunk);

void pushConstant(Value value);
Value popConstant();

#endif