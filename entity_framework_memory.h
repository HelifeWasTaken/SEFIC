#ifndef ENTITY_FRAMEWORK_MEMORY_H
    #define ENTITY_FRAMEWORK_MEMORY_H

    #ifdef __cplusplus
        extern "C" {
            #include <cstddef>
    #else
            #include <stddef.h>
    #endif

            #include "entity_framework_macros.h"

            void *entity_aligned_malloc(size_t size);
            void *entity_aligned_calloc(size_t num, size_t size);
            void entity_aligned_free(void *ptr);
            void *entity_aligned_realloc(void *ptr, size_t size);
            void *entity_aligned_recalloc(void *ptr, size_t old_byte_size, size_t new_byte_size);

            void *entity_memset(void *ptr, int value, size_t num);
            void *entity_memcpy(void *destination, const void *source, size_t num);
            void *entity_memmove(void *destination, const void *source, size_t num);
            int entity_memcmp(const void *ptr1, const void *ptr2, size_t num);

    #ifdef __cplusplus
        }
    #endif


#endif // ENTITY_FRAMEWORK_MEMORY_H
