/*
 * Name:        Mike Nieuweboer
 * Date:        Dec 9 2024
 *
 * Generic implementation for a dynamic array with amortized time complexity 0(1) for insertion.
 * Allows for operations such as appending, popping and searching of values.
 */

#include <stdlib.h>
#include "dynarr_void.h"


struct void_array {
    void **data;
    size_t capacity;
    size_t size;
} typedef void_array;

/*
 * See dynarr_int.h
 */
void_array *void_array_init(size_t capacity) {
    void_array *a = malloc(sizeof(void_array));
    if (!a) return NULL;

    a->data = malloc(sizeof(void *) * capacity);
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
static int array_resize(void_array *a) {
    if (!a) return 1;
    a->capacity = 2 * a->capacity + 1;
    a->data = realloc(a->data, a->capacity * sizeof(void *));
    if (!a->data) return 1;
    return 0;
}

/*
 * See dynarr_int.h
 */
int void_array_append(void_array *a, void *value) {
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
int void_array_get(void_array *a, size_t index, void **element) {
    if (!a || index < 0 || index >= a->size) {
        return 1;
    }
    *element = a->data[index];
    return 0;
}

/*
 * See dynarr_int.h
 */
int void_array_remove(void_array *a, size_t index) {
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
int void_array_pop(void_array *a, void **element) {
    if (!a) return 1;
    if (!a->size) return 2;
    *element = a->data[--(a->size)];
    return 0;
}

/*
 * See dynarr_int.h
 */
size_t void_array_size(void_array *a) {
    if (!a) return 0;
    return a->size;
}

/*
 * See dynarr_int.h
 */
void void_array_free(void_array *a) {
    if (!a) return;
    free(a->data);
    free(a);
}
