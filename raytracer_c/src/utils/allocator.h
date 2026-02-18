/**
 * allocator.h - Memory allocation utilities
 * 
 * Provides memory pooling for Phase 5 optimization.
 * Also includes some helper functions for safe allocation.
 */

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>
#include <stdlib.h>

/* ============================================================================
   SAFE ALLOCATION WRAPPERS
   ============================================================================ */

/**
 * Safe malloc wrapper that exits on failure.
 * @param size      Bytes to allocate
 * @param label     Label for error message
 * @return          Allocated memory (never NULL)
 */
void* xmalloc(size_t size, const char *label);

/**
 * Safe realloc wrapper.
 */
void* xrealloc(void *ptr, size_t size, const char *label);

/* ============================================================================
   MEMORY POOL (Phase 5.5)
   ============================================================================ */

/**
 * Memory pool structure for batch allocation.
 * Allocates large block upfront, doles out small pieces.
 * Useful for avoiding fragmentation in rendering loop.
 */
typedef struct {
    void *buffer;       // Large buffer
    size_t total_size;  // Total buffer size
    size_t used;        // Bytes used so far
    size_t peak;        // Peak usage (for stats)
} memory_pool;

/**
 * Create memory pool.
 * @param size      Pool size in bytes
 * @return          Allocated pool
 */
memory_pool pool_create(size_t size);

/**
 * Allocate from pool.
 * @param pool      Pool
 * @param size      Bytes to allocate
 * @return          Pointer (or NULL if pool full)
 * 
 * WARNING: Returned memory is not zero-initialized.
 * Alignment is not guaranteed (uses byte alignment).
 */
void* pool_alloc(memory_pool *pool, size_t size);

/**
 * Reset pool (clear all allocations).
 * @param pool      Pool
 * 
 * Does NOT free the underlying buffer.
 * Useful for per-frame allocations.
 */
void pool_reset(memory_pool *pool);

/**
 * Destroy pool and free buffer.
 * @param pool      Pool
 */
void pool_destroy(memory_pool *pool);

/**
 * Get pool statistics.
 * @param pool      Pool
 * @param used      (out) Bytes currently used
 * @param peak      (out) Peak bytes ever used
 * @param total     (out) Total pool size
 */
void pool_stats(const memory_pool *pool, size_t *used, size_t *peak, size_t *total);

/* ============================================================================
   GLOBAL POOL (Phase 5.5)
   ============================================================================ */

/**
 * Initialize global memory pool.
 * @param size      Pool size in bytes
 */
void global_pool_init(size_t size);

/**
 * Allocate from global pool.
 */
void* global_pool_alloc(size_t size);

/**
 * Reset global pool.
 */
void global_pool_reset(void);

/**
 * Cleanup global pool.
 */
void global_pool_cleanup(void);

#endif // ALLOCATOR_H
