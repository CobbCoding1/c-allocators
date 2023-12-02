#include <stdio.h>
#include <stdlib.h>
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
    };
    return arena;
}

void *arena_alloc(Arena *arena, size_t size) {
    assert(arena->size + size <= arena->capacity);
    uint8_t *data = &arena->data[arena->size];
    arena->size += size;
    return data;
}

void *arena_reset(Arena *arena) {
    arena->size = 0;
}

void *arena_free(Arena *arena) {
    arena->capacity = 0;
    arena->size = 0;
    free(arena->data);
}

void print_arena(const Arena *arena) {
    printf("capacity: %zu, size: %zu, data ptr: %p\n", arena->capacity, arena->size, arena->data);
}

int main() {
    Arena arena = arena_init(1024);
    void *ptr = arena_alloc(&arena, 18);
    void *ptr2 = arena_alloc(&arena, 10);
    void *ptr3 = arena_alloc(&arena, 218);
    void *ptr3 = arena_alloc(&arena, 1000);
    print_arena(&arena);
    arena_free(&arena);
}
