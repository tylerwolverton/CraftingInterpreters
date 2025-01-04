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
}

void resetChunk(Chunk* chunk)
{
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    initChunk(chunk);
}

void writeChunk(Chunk* chunk, uint8_t byte)
{
    if(chunk->count == chunk->capacity)
    {
        int oldCapacity = chunk->capacity;
        chunk->capacity = growCapacity(chunk->capacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity * CHUNK_CAPACITY_GROWTH_FACTOR);
    }

    // Write byte and increment count
    chunk->code[chunk->count++] = byte;
}