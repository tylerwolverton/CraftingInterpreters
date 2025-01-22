#include "chunk.h"
#include "memory.h"

int CHUNK_CAPACITY_INITIAL_SIZE = 8;
int CHUNK_CAPACITY_GROWTH_FACTOR = 2;

//-------------------------------------------------------------------------------
// Static local functions
static int growCapacity(int oldCapacity)
{
    if(oldCapacity < CHUNK_CAPACITY_INITIAL_SIZE)
    {
        return CHUNK_CAPACITY_INITIAL_SIZE;
    }

    return oldCapacity * CHUNK_CAPACITY_GROWTH_FACTOR;
}

//-------------------------------------------------------------------------------
void initChunk(Chunk* chunk)
{
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->lineNums = NULL;
    initValueArray(&(chunk->constants));
}

void resetChunk(Chunk* chunk)
{
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(int, chunk->lineNums, chunk->capacity);
    resetValueArray(&(chunk->constants));
    initChunk(chunk);
}

void writeToChunk(Chunk* chunk, uint8_t byte, int lineNum)
{
    if(chunk->count == chunk->capacity)
    {
        int oldCapacity = chunk->capacity;
        chunk->capacity = growCapacity(chunk->capacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity * CHUNK_CAPACITY_GROWTH_FACTOR);
    
        chunk->lineNums = GROW_ARRAY(int, chunk->lineNums, oldCapacity, chunk->capacity);
    }

    // Write byte and increment count
    chunk->code[chunk->count] = byte;
    chunk->lineNums[chunk->count] = lineNum;
    chunk->count++;
}

int writeConstantToChunk(Chunk* chunk, Value value, int lineNum)
{
    appendToValueArray(&(chunk->constants), value);
    writeToChunk(chunk, OP_CONSTANT, lineNum);
    writeToChunk(chunk, chunk->constants.count - 1, lineNum);
    return chunk->constants.count - 1;
}
