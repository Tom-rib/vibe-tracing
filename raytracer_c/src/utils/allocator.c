/**
 * allocator.c - Memory allocation implementation
 */

#include "allocator.h"
#include <stdio.h>

/* ============================================================================
   SAFE ALLOCATION WRAPPERS
   ============================================================================ */

void* xmalloc(size_t size, const char *label) {
    void *ptr = malloc(size);
    if (!ptr && size > 0) {
        fprintf(stderr, "Fatal: malloc failed for %s (%zu bytes)\n", label, size);
        exit(1);
    }
    return ptr;
}

void* xrealloc(void *ptr, size_t size, const char *label) {
    void *new_ptr = realloc(ptr, size);
    if (!new_ptr && size > 0) {
        fprintf(stderr, "Fatal: realloc failed for %s (%zu bytes)\n", label, size);
        exit(1);
    }
    return new_ptr;
}

/* ============================================================================
   MEMORY POOL IMPLEMENTATION
   ============================================================================ */

memory_pool pool_create(size_t size) {
    return (memory_pool){
        .buffer = malloc(size),
        .total_size = size,
        .used = 0,
        .peak = 0
    };
}

void* pool_alloc(memory_pool *pool, size_t size) {
    if (pool->used + size > pool->total_size) {
        return NULL;  // Pool full
    }
    
    void *ptr = (char *)pool->buffer + pool->used;
    pool->used += size;
    
    if (pool->used > pool->peak) {
        pool->peak = pool->used;
    }
    
    return ptr;
}

void pool_reset(memory_pool *pool) {
    pool->used = 0;
}

void pool_destroy(memory_pool *pool) {
    if (pool->buffer) {
        free(pool->buffer);
        pool->buffer = NULL;
    }
    pool->total_size = 0;
    pool->used = 0;
    pool->peak = 0;
}

void pool_stats(const memory_pool *pool, size_t *used, size_t *peak, size_t *total) {
    if (used) *used = pool->used;
    if (peak) *peak = pool->peak;
    if (total) *total = pool->total_size;
}

/* ============================================================================
   GLOBAL POOL
   ============================================================================ */

static memory_pool g_pool = {0};
static int g_pool_initialized = 0;

void global_pool_init(size_t size) {
    if (g_pool_initialized) {
        return;
    }
    
    g_pool = pool_create(size);
    g_pool_initialized = 1;
}

void* global_pool_alloc(size_t size) {
    if (!g_pool_initialized) {
        return malloc(size);  // Fallback to regular malloc
    }
    
    void *ptr = pool_alloc(&g_pool, size);
    if (!ptr) {
        fprintf(stderr, "Warning: global pool exhausted, falling back to malloc\n");
        return malloc(size);
    }
    return ptr;
}

void global_pool_reset(void) {
    if (g_pool_initialized) {
        pool_reset(&g_pool);
    }
}

void global_pool_cleanup(void) {
    if (g_pool_initialized) {
        pool_destroy(&g_pool);
        g_pool_initialized = 0;
    }
}
