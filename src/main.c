#include <stdio.h>
#include <stdlib.h>

struct DynamicArray {
    int* start;
    int* capacity;
    int* end;
};

/*
 * This only returns 0 on an error case, otherwise return value is 1 on input 0 and 1, and otherwise the next 2^n such that 2^{n - 1} < x <= 2^n
 * We guarantee that returnValue >= inputValue, with equality only if n = 2^k for some k >= 1
 * There are no overflow checks, so be careful that n is not too large.
 */
static size_t next_power_of_2(const size_t n) {
    if(n == 0) return 0;
    if ((n & (n - 1)) == 0) return n;

    size_t cmp = 1;
    while(cmp < n) cmp *= 2;
    return cmp;
}

/*
 * It is the users responsibility to free arr.start
 */
struct DynamicArray DynamicArray_init(const size_t num_elements) {
    struct DynamicArray arr;

    arr.start = (int*)malloc(num_elements * sizeof(int));
    if(arr.start == NULL) {
        fprintf(stderr, "Memory Allocation Failed!\n");
        exit(EXIT_FAILURE);
    }

    arr.capacity = arr.start + next_power_of_2(num_elements);
    arr.end = arr.start + num_elements;

    // Initialize array values to 0
    for(int* addr = arr.start; addr < arr.capacity; addr++) *addr = 0;

    return arr;
}

int DynamicArray_get(const struct DynamicArray* arr, const size_t idx) {
    if(arr->start + idx >= arr->end) {
        fprintf(stderr, "Index out of bounds: %zu\n", idx);
        exit(EXIT_FAILURE);
    }
    return *(arr->start + idx);
}

/*
 * While this doesn't modify the struct pointers, it does, of course, change the underlying memory so it's not const in that way.
 */
void DynamicArray_set(const struct DynamicArray* arr, const size_t idx, const int value) {
    if(arr->start + idx >= arr->end) {
        fprintf(stderr, "Index out of bounds: %zu\n", idx);
        exit(EXIT_FAILURE);
    }
    *(arr->start + idx) = value;
}

void DynamicArray_push_back(struct DynamicArray* arr, const int value) {
    // Resize with factor 2
    if(arr->end == arr->capacity) {
        const size_t current_size = arr->end - arr->start;
        const size_t new_capacity = (arr->capacity - arr->start) * 2;

        int* new_start = realloc(arr->start, new_capacity * sizeof(int));
        if(new_start == NULL) {
            fprintf(stderr, "Memory reallocation failed!\n");
            exit(EXIT_FAILURE);
        }

        arr->start = new_start;
        arr->end = arr->start + current_size;
        arr->capacity = arr->start + new_capacity;
    }
    *arr->end = value;
    arr->end += 1;
}


int main() {
    const size_t num_elements = 7;
    struct DynamicArray arr = DynamicArray_init(num_elements);

    for(size_t i = 0; i < num_elements; i++) printf("arr.start + %zu : %p -> %d\n", i, arr.start + i, *(arr.start + i));

    printf("End pointer: %p\n", arr.end);
    printf("Capacity pointer: %p\n", arr.capacity);

    // Free allocated memory
    free(arr.start);
    arr.start = NULL;

    return EXIT_SUCCESS;
}