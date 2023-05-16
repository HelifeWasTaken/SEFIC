#ifndef ENTITY_FRAMEWORK_H
    #define ENTITY_FRAMEWORK_H

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "entity_framework_macros.h"
            #include "entity_framework_memory.h"
            #include "entity_framework_types.h"

            // entity_metadata
            void new_entity_metadata(entity_metadata_t *metadata, entity_type_t type, entity_constructor_t constructor, entity_destructor_t destructor, entity_update_t update);
            bool new_entity_metadata_registry(entity_metadata_registry_t *registry, entity_size_t capacity);

            // entity metadata registry
            void delete_entity_metadata_registry(entity_metadata_registry_t *registry);
            bool add_entity_metadata_registry(entity_metadata_registry_t *registry, entity_metadata_t *metadata);

            // entity
            bool _internal_new_entity(entity_manager_t *entity_manager, entity_t *entity, entity_metadata_t *metadata, entity_id_t id, va_list args);
            void _internal_delete_entity(entity_manager_t *entity_manager, entity_t *entity);

            // entity manager
            void new_entity_manager(entity_manager_t *entity_manager, entity_size_t capacity);
            void delete_entity_manager(entity_manager_t *entity_manager);
            bool add_entity(entity_manager_t *entity_manager, entity_type_t type, ...);
            bool delete_entity(entity_manager_t *entity_manager, entity_t *entity);
            void flush_entity_manager_delete_queue(entity_manager_t *entity_manager);
            void update_entity_manager(entity_manager_t *entity_manager, float delta_time);
            bool register_new_entity(entity_manager_t *entity_manager, entity_type_t type, entity_constructor_t constructor, entity_destructor_t destructor, entity_update_t update);

    #ifdef __cplusplus
        }
    #endif

#endif // ENTITY_FRAMEWORK_H
