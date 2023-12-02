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
    while(current != NULL) {
        current->size = 0;
        current = current->next;
    }
}

void arena_free(Arena *arena) {
    free(arena->data);
    arena->capacity = 0;
    arena->size = 0;
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
    Arena *current = arena;
    while(current != NULL) {
        printf("capacity: %zu, size: %zu, data ptr: %p -> ", current->capacity, current->size, current->data);
        current = current->next;
    }
    printf("NULL\n");
}

int main(void) {
    Arena arena = arena_init(1024);
    void *ptr = arena_alloc(&arena, 18);
    char *ptr2 = arena_alloc(&arena, 11);
    void *ptr3 = arena_alloc(&arena, 218);
    void *ptr4 = arena_alloc(&arena, 1000);
    void *ptr5 = arena_alloc(&arena, 1023);
    void *ptr6 = arena_alloc(&arena, 1000);
    print_arena(&arena);
    arena_free(&arena);
    print_arena(&arena);
}
