#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

typedef uint8_t byte;
#define ALIGNMENT sizeof(size_t)

typedef struct {
    size_t capacity;
    size_t size;
    byte *data;
} Stack;

void *custom_realloc(void *pointer, size_t size) {
    void *ptr = realloc(pointer, sizeof(byte) * size);
    if(ptr == NULL) {
        fprintf(stderr, "error: buy more ram\n");
        exit(1);
    }
    return ptr;
}

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
    stack->data = realloc(stack->data, sizeof(byte) * capacity);
    return stack->data;
}

// allocates the size passed + 8 more (sizeof(size_t), ALLIGNMENT) because it has to store the size as well
void *stack_push(Stack *stack, size_t size) {
    if(stack->capacity < size || stack->size + size > stack->capacity || stack == NULL) {
        return NULL;
    }
    byte *data = &stack->data[stack->size];
    stack->size += size;
    *(size_t*)stack->data = size;
    stack->size += ALIGNMENT;
    return data;
}

int stack_pop(Stack *stack) {
    if(stack->size < ALIGNMENT || stack == NULL) {
        return 1;
    }
    size_t amount = *(size_t*)&stack->data[stack->size - ALIGNMENT];
    stack->size -= amount + ALIGNMENT;
    return 0;
}

void stack_reset(Stack *stack) {
    if(stack == NULL) {
        return;
    }
    stack->size = 0;
}

void stack_free(Stack *stack) {
    if(stack == NULL) {
        return;
    }
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
    int *value = stack_push(&stack, 12);
    char *str = stack_push(&stack, sizeof(char) * 15);
    strncpy(str, "Hello, world!\n", 15);
    stack_push(&stack, 100);
    stack_print(&stack);
    *value = 12;
    stack_print(&stack);
    printf("%d\n", *value);
    printf(str);
    stack_free(&stack);
}
