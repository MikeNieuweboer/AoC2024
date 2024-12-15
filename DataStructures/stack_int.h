/*
 * TODO Header
 */

#ifndef STACK_INT_H
#define STACK_INT_H

#include <stddef.h>

typedef struct int_stack int_stack;

/*
 * TODO COMMENT
 */
int_stack *int_stack_init(size_t capacity);

/*
 * TODO COMMENT
 */
int int_stack_append(int_stack *s, int value);

/*
 * TODO COMMENT
 */
int int_stack_pop(int_stack *s, int *element);

/*
 * TODO COMMENT
 */
size_t int_stack_size(int_stack *s);

/*
 * TODO COMMENT
 */
void int_stack_free(int_stack *s);

#endif