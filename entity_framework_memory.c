#include "entity_framework_memory.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <malloc.h>

#if ENTITY_FRAMEWORK_IS_UNIX == 1
    #define ALIGNED_ALLOC(size) aligned_alloc(ENTITY_ALIGNMENT, size)
    #define ALIGNED_FREE(ptr) free(ptr)
#else
    #define ALIGNED_ALLOC(size) _aligned_malloc(size, ENTITY_ALIGNMENT)
    #define ALIGNED_FREE(ptr) _aligned_free(ptr)
#endif

void *entity_aligned_malloc(size_t size)
{
    return ALIGNED_ALLOC(size);
}

void *entity_aligned_calloc(size_t num, size_t size)
{
    void *ptr = entity_aligned_malloc(num * size);
    if (ptr)
        memset(ptr, 0, num * size);
    return ptr;
}

void entity_aligned_free(void *ptr)
{
    ALIGNED_FREE(ptr);
}

void *entity_aligned_realloc(void *ptr, size_t size)
{
#if ENTITY_FRAMEWORK_IS_UNIX == 1
    void *new_ptr;

    #if defined(ENTITY_FRAMEWORK_ALLOW_UNALIGNED_REALLOC)
    {
        new_ptr = realloc(ptr, size);
        if (!new_ptr) {
            entity_aligned_free(ptr);
            return NULL;
        }
    }
    #else
    {
        new_ptr = ALIGNED_ALLOC(size);
        if (!new_ptr) {
            entity_aligned_free(ptr);
            return NULL;
        }

        memcpy(new_ptr, ptr, size);
        entity_aligned_free(ptr);
    }
    #endif
    return new_ptr;
#else
    // windows specific
    return _aligned_realloc(ptr, size, ENTITY_ALIGNMENT);
#endif
}

void *entity_aligned_recalloc(void *ptr, size_t old_byte_size, size_t new_byte_size)
{
    uint8_t *new_ptr = (uint8_t*)entity_aligned_realloc(ptr, new_byte_size);
    if (new_ptr)
        entity_memset(new_ptr + old_byte_size, 0, new_byte_size - old_byte_size);
    return (void*)new_ptr;
}

void *entity_memset(void *ptr, int value, size_t num)
{
    return memset(ptr, value, num);
}

void *entity_memcpy(void *dest, const void *src, size_t num)
{
    return memcpy(dest, src, num);
}

void *entity_memmove(void *dest, const void *src, size_t num)
{
    return memmove(dest, src, num);
}

int entity_memcmp(const void *ptr1, const void *ptr2, size_t num)
{
    return memcmp(ptr1, ptr2, num);
}

#undef ALIGNED_ALLOC
#undef ALIGNED_FREE
