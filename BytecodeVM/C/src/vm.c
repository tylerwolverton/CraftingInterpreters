#include "common.h"
#include "compiler.h"
#include "debug.h"
#include "object.h"
#include "memory.h"
#include "value.h"
#include "vm.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

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

static bool isFalsey(Value value) 
{
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

static void concatenate() 
{
    ObjString* b = AS_STRING(popConstant());
    ObjString* a = AS_STRING(popConstant());

    int length = a->length + b->length;
    char* chars = ALLOCATE(char, length + 1);
    memcpy(chars, a->chars, a->length);
    memcpy(chars + a->length, b->chars, b->length);
    chars[length] = '\0';

    ObjString* result = takeString(chars, length);
    pushConstant(OBJ_VAL(result));
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
    vm.objects = NULL;
    initTable(&vm.strings);
}

void resetVM() 
{
    freeTable(&vm.strings);
    freeObjects();
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

            case OP_NIL:        pushConstant(NIL_VAL); break;
            case OP_TRUE:       pushConstant(BOOL_VAL(true)); break;
            case OP_FALSE:      pushConstant(BOOL_VAL(false)); break;
            case OP_ADD:        
            {
                if (IS_STRING(peekConstant(0)) && IS_STRING(peekConstant(1))) {
                    concatenate();
                } 
                else if (IS_NUMBER(peekConstant(0)) && IS_NUMBER(peekConstant(1))) 
                {
                    double b = AS_NUMBER(popConstant());
                    double a = AS_NUMBER(popConstant());
                    pushConstant(NUMBER_VAL(a + b));
                } 
                else 
                {
                    runtimeError(
                        "Operands must be two numbers or two strings.");
                        return INTERPRET_RUNTIME_ERROR;
                }
                break;
            }
            case OP_SUBTRACT:   BINARY_OP(NUMBER_VAL, -); break;
            case OP_MULTIPLY:   BINARY_OP(NUMBER_VAL, *); break;
            case OP_DIVIDE:     BINARY_OP(NUMBER_VAL, /); break;
            
            case OP_EQUAL: 
            {
                Value b = popConstant();
                Value a = popConstant();
                pushConstant(BOOL_VAL(valuesEqual(a, b)));
                break;
            }
            case OP_GREATER:  BINARY_OP(BOOL_VAL, >); break;
            case OP_LESS:     BINARY_OP(BOOL_VAL, <); break;

            case OP_NOT:
            {
                pushConstant(BOOL_VAL(isFalsey(popConstant())));
                break;
            }

            case OP_NEGATE: 
            {
                if (!IS_NUMBER(peekConstant(0))) 
                {
                    runtimeError("Operand must be a number.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                pushConstant(NUMBER_VAL(-AS_NUMBER(popConstant())));
                break;
            }

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



