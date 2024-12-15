/*
 * TODO Header
 */

#include <stdlib.h>

#include "stack_int.h"
#include "dynarr_int.h"

struct int_stack {
    int_array *a;
} typedef int_stack;

/*
 * See stack_int.h
 */
int_stack *int_stack_init(size_t capacity) {
    int_stack *s = (int_stack *)malloc(sizeof(int_stack));
    if (!s) return NULL;
    s->a = int_array_init(capacity);
    if (!s->a) {
        free(s);
        return NULL;
    }
    return s;
}

/*
 * See stack_int.h
 */
int int_stack_append(int_stack *s, int value) {
    if (!s) return 1;
    return int_array_append(s->a, value);
}

/*
 * See stack_int.h
 */
int int_stack_pop(int_stack *s, int *element) {
    if (!s) return 1;
    return int_array_pop(s->a, element);
}

/*
 * See stack_int.h
 */
size_t int_stack_size(int_stack *s) {
    if (!s) return 0;
    return int_array_size(s->a);
}

/*
 * See stack_int.h
 */
void int_stack_free(int_stack *s) {
    if (!s) return;
    int_array_free(s->a);
    free(s);
}
