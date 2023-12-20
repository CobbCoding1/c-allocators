#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

typedef uint8_t byte;

void *custom_realloc(void *pointer, size_t size) {
    void *ptr = realloc(pointer, sizeof(byte) * size);
    if(ptr == NULL) {
        fprintf(stderr, "error: buy more ram");
        exit(1);
    }
    return ptr;
}

typedef struct {
    size_t capacity;
    size_t size;
    byte *data;
} Stack;

Stack stack_init(size_t capacity) {
    void *data = custom_realloc(NULL, sizeof(byte) * capacity);
    Stack stack = {
        .capacity = capacity,
        .size = 0,
        .data = data,
    };
    return stack;
}

void *stack_realloc(Stack *stack, size_t capacity) {
    stack->capacity = capacity;
    stack->data = realloc(stack->data, capacity);
    return stack->data;
}

void *stack_push(Stack *stack, size_t size) {
    if(stack->capacity < size) {
        return NULL;
    }
    byte *data = &stack->data[stack->size];
    stack->size += size;
    stack->data[stack->size] = size;
    stack->size += sizeof(size_t);
    return data;
}

int stack_pop(Stack *stack) {
    if(stack->size < 8) {
        return 1;
    }
    size_t amount = *(size_t*)&stack->data[stack->size - sizeof(size_t)];
    stack->size -= amount + sizeof(size_t);
    return 0;
}

void stack_reset(Stack *stack) {
    stack->size = 0;
}

void stack_free(Stack *stack) {
    free(stack->data);
    stack->capacity = 0;
    stack->size = 0;
}

void stack_print(const Stack *stack) {
    printf("stack size: %zu, stack capacity: %zu\n", stack->size, stack->capacity);
}

int main(void) {
    Stack stack = stack_init(1000);
    stack_print(&stack);
    stack_realloc(&stack, 2000);
    int *value = stack_push(&stack, 12);
    stack_push(&stack, 100);
    stack_print(&stack);
    *value = 12;
    stack_print(&stack);
    printf("%d\n", *value);
    stack_free(&stack);
}
