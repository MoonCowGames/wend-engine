#ifndef __WEND_MEMORY_ARENA_H__
#define __WEND_MEMORY_ARENA_H__

#include <stdlib.h>
#include <string.h>

#include "..\misc\typedefs.h"

namespace Memory
{
  /// @brief Data structure for memory management utilities.
  ///
  /// Memory arena is allocated memory from the OS, which can then be served
  /// ad hoc to any requesting structures. Arenas simplify memory management by 
  /// having only one alloc and one free for a collection of memory, which also 
  /// reduces performance overhead from calling to the OS.
  struct Arena
  {
    /// @brief Position at the start of free memory.
    uint64 position = 0;

    /// @brief Size of allocated space.
    uint64 size = 0;

    /// @brief Allocated space for the arena.
    void* storage = 0;
  };

  /// @brief Allocates storage for the arena as a whole.
  /// @param size Size of the arena.
  /// @return Ptr to arena.
  void ArenaAlloc(Arena* arena, uint64 size);

  /// @brief Allocates storage for an arena as a child of another arena.
  /// @param parentArena Arena that the sub-arena belongs to.
  /// @param size Size of the sub arena.
  /// @return Ptr to sub-arena.
  void SubArenaAlloc(Arena* subArena, Arena* parentArena, uint64 size);

  /// @brief Releases storage of the arena back to OS.
  /// @param arena Arena to free.
  void ArenaFree(Arena* arena);

  /// @brief Serves memory to caller.
  /// @param arena The arena to push to.
  /// @param size The size of memory being requested.
  /// @return Ptr to requested memory.
  void* ArenaPush(Arena* arena, uint64 size);

  /// @brief Serves zeroed memory to caller.
  /// @param arena The arena to push to.
  /// @param size The size of memory being requested.
  /// @return Ptr to requested memory.
  void* ArenaPushZeroed(Arena* arena, uint64 size);

  /// @brief Releases memory back to the arena.
  /// @param arena The arena to pop from.
  /// @param size The size to pop.
  void ArenaPop(Arena* arena, uint64 size);
  
  /// @brief Returns the starting point of free memory.
  /// @param arena Arena to get position from.
  /// @return Position of free memory.
  uint64 ArenaGetPosition(Arena* arena);

  /// @brief Helper. Sets position of free memory to desired location.
  /// @param arena Arena to set position of.
  /// @param position Desired position.
  void ArenaSetPosition(Arena* arena, uint64 position);

  /// @brief Frees internal memory without deallocating arena.
  /// @param arena Arena to clear.
  void ArenaClear(Arena* arena);
} 


#endif //__WEND_MEMORY_ARENA_H__