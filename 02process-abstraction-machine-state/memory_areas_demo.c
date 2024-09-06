#include <stdio.h>
#include <stdlib.h>

// Global variables (stored in Data Segment - Initialized)
int global_initialized = 100;

// Global variables (stored in BSS Segment - Uninitialized)
int global_uninitialized;

// Function to demonstrate stack usage
void stack_demo(int param) {
    // Local variables (stored on Stack)
    int local_var = 20;
    printf("Stack Demo - Parameter: %d, Local Variable: %d\n", param, local_var);
}

// Static variable (stored in Data Segment)
static int static_var = 50;

int main() {
    // Code Segment
    printf("This print statement's code is in the Code Segment\n");

    // Data Segment - Static Area
    printf("Static Variable: %d\n", static_var);

    // Data Segment - Initialized
    printf("Global Initialized: %d\n", global_initialized);

    // BSS Segment
    printf("Global Uninitialized (BSS): %d\n", global_uninitialized);

    // Stack
    stack_demo(10);

    // Heap
    int* heap_var = (int*)malloc(sizeof(int));
    *heap_var = 30;
    printf("Heap Variable: %d\n", *heap_var);

    // Don't forget to free heap memory
    free(heap_var);

    return 0;
}