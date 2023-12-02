#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

typedef struct Arena {
    struct Arena *next;
    size_t capacity;
    size_t size;
    uint8_t *data;
} Arena;

Arena arena_init(size_t capacity) {
    void *data = malloc(sizeof(uint8_t) * capacity);
    Arena arena = {
        .capacity = capacity,
        .size = 0,
        .data = data,
        .next = NULL,
    };
    return arena;
}

void *arena_alloc(Arena *arena, size_t size) {
    assert(arena->capacity >= size);
    Arena *current = arena;
    while(!(current->size + size <= current->capacity)) {
        if(current->next == NULL) {
            Arena *next = malloc(sizeof(Arena));
            Arena initted = arena_init(arena->capacity);
            memcpy(next, &initted, sizeof(Arena));
            current->next = next;
        }
        current = current->next;
    }

    uint8_t *data = &current->data[current->size];
    current->size += size;
    return data;
}

void arena_reset(Arena *arena) {
    Arena *current = arena;
    while(current->next != NULL) {
        current->size = 0;
        current = current->next;
    }
}

void arena_free(Arena *arena) {
    free(arena->data);
    Arena *current = arena->next;
    while(current != NULL) {
        Arena *tmp = current->next;
        free(current->data);
        free(current);
        current = tmp;
    }
    arena->next = NULL;
}

void print_arena(const Arena *arena) {
    printf("capacity: %zu, size: %zu, data ptr: %p\n", arena->capacity, arena->size, arena->data);
}

int main() {
    Arena arena = arena_init(1024);
    void *ptr = arena_alloc(&arena, 18);
    void *ptr2 = arena_alloc(&arena, 10);
    void *ptr3 = arena_alloc(&arena, 218);
    void *ptr4 = arena_alloc(&arena, 1000);
    void *ptr5 = arena_alloc(&arena, 1023);
    void *ptr6 = arena_alloc(&arena, 1000);
    arena_reset(&arena);
    print_arena(arena.next);
    arena_free(&arena);
}
