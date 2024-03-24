#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

void *custom_malloc(size_t size) {
    void *ptr = malloc(sizeof(uint8_t) * size);
    if(ptr == NULL) {
        fprintf(stderr, "error: buy more ram");
        exit(1);
    }
    return ptr;
}

typedef struct Arena {
    struct Arena *next;
    size_t capacity;
    size_t size;
    uint8_t *data;
} Arena;

Arena arena_init(size_t capacity) {
    void *data = custom_malloc(sizeof(uint8_t) * capacity);
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
            Arena *next = custom_malloc(sizeof(Arena));
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

void *arena_realloc(Arena *arena, void *old_ptr, size_t old_size, size_t new_size) {
    if (new_size <= old_size) return old_ptr;

    void *new_ptr = arena_alloc(arena, new_size);
    char *new_ptr_char = new_ptr;
    char *old_ptr_char = old_ptr;

    for (size_t i = 0; i < old_size; ++i) {
        new_ptr_char[i] = old_ptr_char[i];
    }

    return new_ptr;
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

void arena_print(const Arena *arena) {
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
    void *ptr_new = arena_realloc(&arena, ptr, 18, 30);
    char *ptr2 = arena_alloc(&arena, 11);
    void *ptr3 = arena_alloc(&arena, 218);
    void *ptr4 = arena_alloc(&arena, 1000);
    void *ptr5 = arena_alloc(&arena, 1023);
    void *ptr6 = arena_alloc(&arena, 1000);
    arena_print(&arena);
    arena_free(&arena);
    arena_print(&arena);
}
