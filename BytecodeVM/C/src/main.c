#include "common.h"
#include "chunk.h"
#include "debug.h"

#include <stdio.h>

int main ( int argc, char*argv[] )
{
    Chunk chunk;
    initChunk(&chunk);
    writeChunk(&chunk, OP_RETURN);
    writeChunk(&chunk, OP_RETURN);

    disassembleChunk(&chunk, "test chunk");

    resetChunk(&chunk);
    return 0;
}
