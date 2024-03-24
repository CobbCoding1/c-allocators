#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

void _arena_init(size_t capacity);
void *arena_alloc(size_t size);
void arena_reset();
void arena_free();
void arena_print();

#define arena_init(size) \
    {_arena_init(size); atexit(arena_free); }

int main(void) {
    arena_init(1024);
    void *ptr = arena_alloc(18);
    void *ptr_new = arena_realloc(ptr, 18, 20);
    char *ptr2 = arena_alloc(11);
    void *ptr3 = arena_alloc(218);
    void *ptr4 = arena_alloc(1000);
    void *ptr5 = arena_alloc(1023);
    void *ptr6 = arena_alloc(1000);
    arena_print();
    arena_print();
    //arena_free();
}

typedef struct Arena {
    struct Arena *next;
    size_t capacity;
    size_t size;
    uint8_t *data;
} Arena;

// very secret function)
static Arena* get_local_arena()
{
    static Arena arena = {
            .capacity = 0,
            .size = 0,
            .data = NULL,
            .next = NULL,
    };
    return &arena;
}
void *custom_malloc(size_t size) {
    void *ptr = malloc(sizeof(uint8_t) * size);
    if(ptr == NULL) {
        fprintf(stderr, "error: buy more ram");
        exit(1);
    }
    return ptr;
}
void _arena_init(size_t capacity) {
    Arena *arena = get_local_arena();
    if (arena->data == NULL) {
        arena->data = custom_malloc(sizeof(uint8_t) * capacity);
        arena->capacity = capacity;
    }
}
void *arena_alloc(size_t size) {
    Arena *arena = get_local_arena();
    Arena *current = arena;
    assert(current->capacity >= size);

    while(!(current->size + size <= current->capacity)) {
        if(current->next == NULL) {
            Arena *next = custom_malloc(sizeof(Arena));
            next->capacity = arena->capacity;
            next->size = 0;
            next->next = NULL;
            next->data = custom_malloc(sizeof(uint8_t) * arena->capacity);
            current->next = next;
        }
        current = current->next;
    }

    uint8_t *data = &current->data[current->size];
    current->size += size;
    return data;
}

void *arena_realloc(void *old_ptr, size_t old_size, size_t new_size) {
    if (new_size <= old_size) return old_ptr;

    void *new_ptr = arena_alloc(new_size);
    char *new_ptr_char = new_ptr;
    char *old_ptr_char = old_ptr;

    for (size_t i = 0; i < old_size; ++i) {
        new_ptr_char[i] = old_ptr_char[i];
    }

    return new_ptr;
}

void arena_reset() {
    Arena *current = get_local_arena();
    while (current != NULL) {
        current->size = 0;
        current = current->next;
    }
}

void arena_free() {
    Arena *arena = get_local_arena();
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

void arena_print() {
    Arena *current = get_local_arena();
    while(current != NULL) {
        printf("capacity: %zu, size: %zu, data ptr: %p -> ", current->capacity, current->size, current->data);
        current = current->next;
    }
    printf("NULL\n");
}
