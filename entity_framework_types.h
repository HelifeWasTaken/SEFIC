#ifndef ENTITY_FRAMEWORK_TYPES_H
    #define ENTITY_FRAMEWORK_TYPES_H

    #ifdef __cplusplus
        extern "C" {
            #include <cstdint>
    #else
            #include <stdint.h>
    #endif

            #include <stdarg.h>

            #include <stdbool.h>

            #include "entity_framework_macros.h"

            typedef uint64_t entity_type_t;
            typedef uint64_t entity_id_t;
            typedef void* entity_instance_t;

            typedef uint8_t entity_byte_t;
            typedef size_t entity_size_t;

            struct entity;
            struct entity_manager;

            typedef void (*entity_constructor_t)(struct entity_manager *, struct entity *, va_list);
            typedef void (*entity_destructor_t)(struct entity_manager *, struct entity *);
            typedef void (*entity_update_t)(struct entity_manager *, struct entity *, float);

            ENTITY_STRUCT_DECL(entity_metadata, entity_metadata_t, {
                entity_type_t type;
                entity_constructor_t constructor;
                entity_destructor_t destructor;
                entity_update_t update;
                entity_byte_t set;
            });

            ENTITY_STRUCT_DECL(entity_metadata_registry, entity_metadata_registry_t, {
                entity_metadata_t *metadata;
                entity_size_t count;
                entity_size_t capacity;
            });

            ENTITY_STRUCT_DECL(entity, entity_t, {
                entity_metadata_t *metadata;

                entity_id_t id;
                entity_instance_t instance;

                entity_t *prev;
                entity_t *next;
            });

            ENTITY_STRUCT_DECL(entity_list_to_free, entity_list_to_free_t, {
                entity_t *to_free;
                entity_list_to_free_t *next;
            });

            ENTITY_STRUCT_DECL(entity_manager, entity_manager_t, {
                entity_t *first;
                entity_t *last;

                entity_id_t entity_count;

                entity_metadata_registry_t metadata_registry;
                entity_id_t next_id;
                entity_list_to_free_t *to_free;
            });

    #ifdef __cplusplus
        }
    #endif

#endif // ENTITY_FRAMEWORK_TYPES_H
