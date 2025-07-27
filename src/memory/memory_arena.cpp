#include "memory_arena.h"

void Memory::ArenaAlloc(Arena* arena, uint64 size)
{
  if (!arena->storage)
  {
    arena->storage = malloc(size); 
    if (!arena->storage)
    {
      // TODO: Handle/Log error
    }
    arena->size = size;
    arena->position = (uint64)arena->storage;
    // TODO: Zero mem
  }
}

void Memory::SubArenaAlloc(Arena* subArena, Arena* parentArena, uint64 size)
{
  subArena->storage = (void*)parentArena->position;
  parentArena->position += size;
  subArena->size = size;
  subArena->position = (uint64)subArena->storage;
}

void Memory::ArenaFree(Arena* arena)
{
  // TODO: Zero mem
  if (arena->storage)
  {
    free(arena->storage);
    arena->storage = nullptr;
  }
}

void* Memory::ArenaPush(Arena* arena, uint64 size)
{ 
  void* ptr = (void*)arena->position;
  arena->position += size;
  return ptr;
}

void* Memory::ArenaPushZeroed(Arena* arena, uint64 size)
{
  void* ptr = (void*)arena->position;
  // TODO: Zero mem
  arena->position += size;
  return ptr;
}

void Memory::ArenaPop(Arena* arena, uint64 size)
{
  arena->position -= size;
  // TODO: Zero mem
}

void Memory::ArenaSetPosition(Arena* arena, uint64 position)
{
  arena->position = position;
  // TODO: Zero mem
}

void Memory::ArenaClear(Arena* arena)
{
  arena->position = (uint64)arena->storage;
  // TODO: Zero mem
}
