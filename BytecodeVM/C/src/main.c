#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

#include <stdio.h>

int main ( int argc, char*argv[] )
{
    initVM();

    Chunk chunk;
    initChunk(&chunk);

    Value constant = writeConstantToChunk(&chunk, 3.4, 123);
    writeToChunk(&chunk, OP_CONSTANT, 123);
    writeToChunk(&chunk, constant, 123);

    writeToChunk(&chunk, OP_ADD, 123);

    constant = writeConstantToChunk(&chunk, 5.6, 123);
    writeToChunk(&chunk, OP_CONSTANT, 123);
    writeToChunk(&chunk, constant, 123);

    writeToChunk(&chunk, OP_DIVIDE, 123);
    writeToChunk(&chunk, OP_NEGATE, 123);

    writeToChunk(&chunk, OP_RETURN, 123);

    disassembleChunk(&chunk, "test chunk");
    interpretChunk(&chunk);

    resetChunk(&chunk);
    resetVM();
    return 0;
}
