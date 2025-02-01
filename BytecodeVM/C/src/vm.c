#include "common.h"
#include "compiler.h"
#include "debug.h"
#include "value.h"
#include "vm.h"

#include <stdarg.h>
#include <stdio.h>

VM vm; 

static void resetStack() 
{
    vm.stackTop = vm.stack;
}

static void pushConstant(Value value)
{
    *vm.stackTop = value;
    ++vm.stackTop;
}

static Value popConstant()
{
    --vm.stackTop;
    return *vm.stackTop;
}

static Value peekConstant(int distance) 
{
    return vm.stackTop[-1 - distance];
}

static void runtimeError(const char* format, ...) 
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    size_t instruction = vm.ip - vm.chunk->code - 1;
    int line = vm.chunk->lineNums[instruction];
    fprintf(stderr, "[line %d] in script\n", line);
    resetStack();
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
#define BINARY_OP(valueType, op) \
    do { \
      if (!IS_NUMBER(peekConstant(0)) || !IS_NUMBER(peekConstant(1))) { \
        runtimeError("Operands must be numbers."); \
        return INTERPRET_RUNTIME_ERROR; \
      } \
      double b = AS_NUMBER(popConstant()); \
      double a = AS_NUMBER(popConstant()); \
      pushConstant(valueType(a op b)); \
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

            case OP_ADD:      BINARY_OP(NUMBER_VAL, +); break;
            case OP_SUBTRACT: BINARY_OP(NUMBER_VAL, -); break;
            case OP_MULTIPLY: BINARY_OP(NUMBER_VAL, *); break;
            case OP_DIVIDE:   BINARY_OP(NUMBER_VAL, /); break;
            
            case OP_NEGATE: 
                if (!IS_NUMBER(peekConstant(0))) 
                {
                    runtimeError("Operand must be a number.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                pushConstant(NUMBER_VAL(-AS_NUMBER(popConstant())));
                break;
            
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

InterpretResult interpret(const char* source) 
{
    Chunk chunk;
    initChunk(&chunk);

    if (!compile(source, &chunk)) 
    {
        resetChunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }

    vm.chunk = &chunk;
    vm.ip = vm.chunk->code;

    InterpretResult result = run();

    resetChunk(&chunk);
    return result;
}



