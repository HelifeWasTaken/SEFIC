# Simple Updatable Entity Framework

## Introduction

This is a simple framework for creating updatable entities. It is designed to be used for creating simple games using an OOP like style in C

It is a simple POC that I use and is surely not a fully featured framework.

This framework works for both Linux and Windows.

## Memory Management

All the entities are stored in a linked list, and the memory is always aligned on 32bits using aligned_alloc dependent system.

May change in the future to use a custom memory allocator and a contiguous array.

## Usage

### Creating an entity

```c
#include "entity_framework.h"

typedef struct {
    // ...
} Player;

void create_player(struct entity_manager *manager, struct entity *entity, va_list args)
{
    Player *player = malloc(sizeof(Player));

    // ... init player (using args if needed)

    entity->instance = (void *)player;

    // The function is considered successful if entity->instance is not NULL
}

void destroy_player(struct entity_manager *manager, struct entity *entity)
{
    Player *player = (Player *)entity->instance;

    // ... Free player internals if needed

    free(player);
    entity->instance = NULL;
}

void update_player(struct entity_manager *manager, struct entity *entity, float dt)
{
    Player *player = (Player *)entity->instance;

    // ... Update player internals using dt if needed
}

enum types {
    PLAYER,
    // ...
};

int main(void)
{
    entity_manager_t manager;

    // 1 is the number of entities types to allocate at start
    // It is used to avoid reallocating memory too often
    // If you know the number of different entities types you will have,
    // you can set it to that number
    new_entity_manager(&manager, 1); 

    // Register the entity type
    register_new_entity(&manager, PLAYER, create_player, destroy_player, update_player);
    // Create an entity of type PLAYER
    add_entity(&manager, PLAYER /*, args if needed */);

    // ... Update loop
    while (/* ... */) {
        float dt = /* ... */;
        update_entities(&manager, dt);
    }

    // ... Free entities
    delete_entity_manager(&manager);
}
```
