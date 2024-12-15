/*
 * Name:        Mike Nieuweboer
 * Date:        Dec 9 2024
 *
 * Generic implementation for a dynamic array with amortized time complexity 0(1) for insertion.
 * Allows for operations such as appending, popping and searching of values.
 */

#include <stdlib.h>
#include <stdio.h>
#include "dynarr_int.h"


struct int_array {
    int *data;
    size_t capacity;
    size_t size;
} typedef int_array;

/*
 * See dynarr_int.h
 */
int_array *int_array_init(size_t capacity) {
    int_array *a = malloc(sizeof(int_array));
    if (!a) return NULL;

    a->data = malloc(sizeof(int) * capacity);
    if (!a->data) {
        free(a);
        return NULL;
    }
    a->size = 0;
    a->capacity = capacity;
    return a;
}

/*
 * TODO COMMENT
 */
static int array_resize(int_array *a) {
    if (!a) return 1;
    a->capacity = 2 * a->capacity + 1;
    a->data = realloc(a->data, a->capacity * sizeof(int));
    if (!a->data) return 1;
    return 0;
}

/*
 * See dynarr_int.h
 */
int int_array_append(int_array *a, int value) {
    if (!a) return -1;

    if (a->size >= a->capacity) {
        if (array_resize(a)) return -1;
    }
    a->data[a->size++] = value;
    return a->size - 1;
}

/*
 * See dynarr_int.h
 */
int int_array_get(int_array *a, size_t index, int *element) {
    if (!a || index < 0 || index >= a->size) {
        return 1;
    }
    *element = a->data[index];
    return 0;
}

/*
 * See dynarr_int.h
 */
int int_array_remove(int_array *a, size_t index) {
    if (!a || index < 0 || index >= a->size) {
        return 1;
    }

    // Shift all elements after deleted one to the left.
    for (int i = index; i < a->size; i++) {
        a->data[i] = a->data[i + 1];
    }
    a->size--;
    return 0;
}

/*
 * See dynarr_int.h
 */
int int_array_pop(int_array *a, int *element) {
    if (!a) return 1;
    if (!a->size) return 2;
    *element = a->data[--(a->size)];
    return 0;
}

/*
 * See dynarr_int.h
 */
size_t int_array_size(int_array *a) {
    if (!a) return 0;
    return a->size;
}

/*
 * See dynarr_int.h
 */
void int_array_free(int_array *a) {
    if (!a) return;
    free(a->data);
    free(a);
}

/*
 * See dynarr_int.h
 */
void int_array_bubble_sort(int_array *a, int descending) {
    if (!a) return;
    int changed = 1;
    while (changed) {
        changed = 0;
        for (int i = 0; i < a->size - 1; i++) {
            if ((!descending && a->data[i] > a->data[i + 1]) ||
                (descending && a->data[i] < a->data[i + 1])
            ) {
                a->data[i] += a->data[i + 1];
                a->data[i + 1] = a->data[i] - a->data[i + 1];
                a->data[i] -= a->data[i + 1];
                changed = 1;
            }
        }
    }
}

/*
 * See dynarr_int.h
 */
void int_array_print(int_array *a) {
    if (!a) return;
    printf("Array of size %lu:\n", a->size);
    for (int i = 0; i < a->size; i++) {
        printf("%d, ", a->data[i]);
    }
    printf("\n");
}
