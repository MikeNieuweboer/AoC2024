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


typedef struct int_array int_array;

/*
 * Initialises an empty dynamic array
 * In:
 * capacity, the amount of space allocated for values
 *
 * Out:
 * The array struct pointer, or NULL on failure.
 */
int_array *int_array_init(size_t capacity);

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
int int_array_append(int_array *a, int value);

/*
 * TODO COMMENT
 */
int int_array_get(int_array *a, size_t index, int *element);

/*
 * TODO COMMENT
 */
int int_array_remove(int_array *a, size_t index);

/*
 * TODO COMMENT
 */
int int_array_pop(int_array *a, int *element);

/*
 * TODO COMMENT
 */
size_t int_array_size(int_array *a);

/*
 * TODO COMMENT
 */
void int_array_free(int_array *a);

/*
 * TODO COMMENT
 */
void int_array_bubble_sort(int_array *a, int descending);

/*
 * TODO COMMENT
 */
void int_array_print(int_array *a);

#endif
