#include "entity_framework.h"

// entity_metadata

void new_entity_metadata(entity_metadata_t *metadata, entity_type_t type, entity_constructor_t constructor, entity_destructor_t destructor, entity_update_t update)
{
    ENTITY_FRAMEWORK_CHECK(
        if (constructor == NULL || destructor == NULL || update == NULL) {
            abort();
        }
    );

    metadata->type = type;
    metadata->constructor = constructor;
    metadata->destructor = destructor;
    metadata->update = update;
    metadata->set = 1;
}

bool new_entity_metadata_registry(entity_metadata_registry_t *registry, entity_size_t capacity)
{
    ENTITY_FRAMEWORK_CHECK(if (capacity == 0) { return false; });

    ENTITY_ALIGNED_VAR(entity_size_t, capacity_bytes);
    capacity_bytes = capacity * sizeof(entity_metadata_t);
    capacity_bytes = ENTITY_ALIGN(capacity_bytes);

    registry->metadata = entity_aligned_calloc(capacity_bytes, 1);

    ENTITY_FRAMEWORK_ALLOC_CHECK(if (registry->metadata == NULL) { return false; });

    registry->count = 0;
    registry->capacity = capacity;
    return true;
}

// entity_metadata_registry

void delete_entity_metadata_registry(entity_metadata_registry_t *registry)
{
    entity_aligned_free(registry->metadata);
    entity_memset(registry, 0, sizeof(entity_metadata_registry_t));
}

bool add_entity_metadata_registry(entity_metadata_registry_t *registry, entity_metadata_t *metadata)
{
    if (metadata->type >= registry->capacity) {
        registry->capacity = metadata->type + 1;

        ENTITY_ALIGNED_VAR(entity_size_t, new_capacity_bytes);
        new_capacity_bytes = registry->capacity * sizeof(entity_metadata_t);
        new_capacity_bytes = ENTITY_ALIGN(new_capacity_bytes);

        registry->metadata = entity_aligned_recalloc(registry->metadata,
            registry->capacity * sizeof(entity_metadata_t),
            new_capacity_bytes);

        ENTITY_FRAMEWORK_ALLOC_CHECK(if (new_metadata == NULL) {
            entity_memset(registry, 0, sizeof(entity_metadata_registry_t));
            return false;
        });
    }

    if (registry->metadata[metadata->type].set)
        return false;
    registry->metadata[metadata->type] = *metadata;
    ++registry->count;
    return true;
}

// entity

bool _internal_new_entity(entity_manager_t *entity_manager, entity_t *entity, entity_metadata_t *metadata, entity_id_t id, va_list args)
{
    entity->metadata = metadata;
    entity->id = id;
    entity->prev = NULL;
    entity->next = NULL;

    entity->metadata->constructor(entity_manager, entity, args);
    if (entity->instance == NULL) {
        return false;
    }

    if (entity_manager->first == NULL) {
        entity_manager->first = entity;
        entity_manager->last = entity;
    } else {
        entity_manager->last->next = entity;
        entity->prev = entity_manager->last;
        entity_manager->last = entity;
    }
    return true;
}

void _internal_delete_entity(entity_manager_t *entity_manager, entity_t *entity)
{
    if (entity->prev == NULL) {
        entity_manager->first = entity->next;
    } else {
        entity->prev->next = entity->next;
    }

    if (entity->next == NULL) {
        entity_manager->last = entity->prev;
    } else {
        entity->next->prev = entity->prev;
    }

    entity->metadata->destructor(entity_manager, entity);
    entity_aligned_free(entity);
    --entity_manager->entity_count;
}

// entity_manager

void new_entity_manager(entity_manager_t *entity_manager, entity_size_t capacity)
{
    entity_manager->first = NULL;
    entity_manager->last = NULL;
    entity_manager->entity_count = 0;
    new_entity_metadata_registry(&entity_manager->metadata_registry, capacity);
    entity_manager->next_id = 0;
    entity_manager->to_free = NULL;
}

void delete_entity_manager(entity_manager_t *entity_manager)
{
    while (entity_manager->first != NULL)
        _internal_delete_entity(entity_manager, entity_manager->first);
}

bool add_entity(entity_manager_t *entity_manager, entity_type_t type, ...)
{
    if (type >= entity_manager->metadata_registry.capacity) {
        return false;
    }

    ENTITY_ALIGNED_VAR(entity_metadata_t *, metadata);
    metadata = entity_manager->metadata_registry.metadata + type;

    if (!metadata->set) {
        return false;
    }

    ENTITY_ALIGNED_VAR(entity_t *, entity);
    entity = entity_aligned_calloc(sizeof(entity_t), 1);

    ENTITY_FRAMEWORK_ALLOC_CHECK(if (entity == NULL) { return false; });

    va_list args;
    va_start(args, type);

    bool success = _internal_new_entity(entity_manager, entity, metadata, entity_manager->next_id++, args);
    if (!success) {
        entity_manager->next_id--;
        entity_aligned_free(entity);
    }

    va_end(args);
    return success;
}

bool delete_entity(entity_manager_t *entity_manager, entity_t *entity)
{
    entity_list_to_free_t *to_free = entity_aligned_calloc(sizeof(entity_list_to_free_t), 1);

    ENTITY_FRAMEWORK_ALLOC_CHECK(if (to_free == NULL) { return false; });

    if (entity_manager->to_free == NULL) {
        entity_manager->to_free = to_free;
    } else {
        to_free->next = entity_manager->to_free;
    }
    to_free->to_free = entity;
    return true;
}

void flush_entity_manager_delete_queue(entity_manager_t *entity_manager)
{
    while (entity_manager->to_free != NULL) {
        entity_list_to_free_t *to_free = entity_manager->to_free;
        entity_t *entity = to_free->to_free;
        entity_manager->to_free = to_free->next;

        _internal_delete_entity(entity_manager, entity);
        entity_aligned_free(to_free);
    }
}

void update_entity_manager(entity_manager_t *entity_manager, float dt)
{
    flush_entity_manager_delete_queue(entity_manager);

    for (entity_t *entity = entity_manager->first; entity != NULL; entity = entity->next)
        entity->metadata->update(entity_manager, entity, dt);
}

bool register_new_entity(entity_manager_t *entity_manager, entity_type_t type, entity_constructor_t constructor, entity_destructor_t destructor, entity_update_t update)
{
    entity_metadata_t metadata;
    metadata.type = type;
    metadata.set = true;
    metadata.constructor = constructor;
    metadata.destructor = destructor;
    metadata.update = update;

    return add_entity_metadata_registry(&entity_manager->metadata_registry, &metadata);
}
