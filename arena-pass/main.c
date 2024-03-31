#define ARENA_IMPLEMENTATION
#include "arena.h"

int main(void) {
    Arena arena = arena_init(ARENA_INIT_SIZE);
    void *ptr = arena_alloc(&arena, 18);
    char *ptr2 = arena_alloc(&arena, 11);
    void *ptr3 = arena_alloc(&arena, 218);
    void *ptr4 = arena_alloc(&arena, 1000);
    void *ptr5 = arena_alloc(&arena, 1023);
    void *ptr6 = arena_alloc(&arena, 1000);
    arena_print(&arena);
    arena_free(&arena);
    arena_print(&arena);
}
