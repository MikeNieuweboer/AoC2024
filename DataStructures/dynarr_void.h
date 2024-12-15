/*
 * Name:        Mike Nieuweboer
 * Date:        Dec 9 2024
 *
 * Generic implementation for a dynamic array with amortized time complexity 0(1) for insertion.
 * Allows for operations such as appending, popping and searching of values.
 */
#ifndef DYNARR_INT_H
#define DYNARR_INT_H

#include <stddef.h>


typedef struct void_array void_array;

/*
 * Initialises an empty dynamic array
 * In:
 * capacity, the amount of space allocated for values
 *
 * Out:
 * The array struct pointer, or NULL on failure.
 */
void_array *void_array_init(size_t capacity);

/*
 * Appends a value to the end of the dynamic array.
 *
 * In:
 * a, the array's struct
 * value, the int to be appended.
 *
 * Out:
 * The index of the element if append was succesful, -1 otherwise.
 */
int void_array_append(void_array *a, void *value);

/*
 * TODO COMMENT
 */
int void_array_get(void_array *a, size_t index, void **element);

/*
 * TODO COMMENT
 */
int void_array_remove(void_array *a, size_t index);

/*
 * TODO COMMENT
 */
int void_array_pop(void_array *a, void **element);

/*
 * TODO COMMENT
 */
size_t void_array_size(void_array *a);

/*
 * TODO COMMENT
 */
void void_array_free(void_array *a);

#endif
