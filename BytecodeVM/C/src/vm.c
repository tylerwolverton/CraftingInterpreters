#include "common.h"
#include "debug.h"
#include "vm.h"

#include <stdio.h>

VM vm; 

static void resetStack() 
{
    vm.stackTop = vm.stack;
}

void initVM() 
{
    resetStack();
}

void resetVM() 
{

}

static InterpretResult run()
{
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
    do { \
        double b = popConstant(); \
        double a = popConstant(); \
        pushConstant(a op b); \
    } while (false)

    for (;;) 
    {
        uint8_t instruction;
        switch (instruction = READ_BYTE()) 
        {
#ifdef DEBUG_TRACE_EXECUTION
            printf("          ");
            for (Value* slot = vm.stack; slot < vm.stackTop; slot++) 
            {
                printf("[ ");
                printValue(*slot);
                printf(" ]");
            }
            printf("\n");

            disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
            case OP_CONSTANT: 
            {
                Value constant = READ_CONSTANT();
                pushConstant(constant);
                break;
            }

            case OP_ADD:      BINARY_OP(+); break;
            case OP_SUBTRACT: BINARY_OP(-); break;
            case OP_MULTIPLY: BINARY_OP(*); break;
            case OP_DIVIDE:   BINARY_OP(/); break;
            
            case OP_NEGATE: pushConstant(-popConstant()); break;
            
            case OP_RETURN: 
            {
                printValue(popConstant());
                printf("\n");
                return INTERPRET_OK;
            }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef READ_BYTE
#undef BINARY_OP
}

InterpretResult interpretChunk(Chunk* chunk) 
{
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run();
}

void pushConstant(Value value)
{
    *vm.stackTop = value;
    ++vm.stackTop;
}

Value popConstant()
{
    --vm.stackTop;
    return *vm.stackTop;
}
