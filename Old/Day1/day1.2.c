#include <stdio.h>
#include <stdlib.h>

#include "DataStructures/dynarr_int.h"

#define BUF_SIZE 1024

struct table {
    int_array *left;
    int_array *right;
} typedef table;

table *read_file(char *file_name) {
    if (!file_name) return NULL;
    table *t = (table *)malloc(sizeof(table));
    if (!t) return NULL;
    t->right = int_array_init(128);
    t->left = int_array_init(128);
    if (!t->left || !t->right) {
        int_array_free(t->left);
        int_array_free(t->right);
        free(t);
        return NULL;
    }
    FILE *f = fopen(file_name, "r");
    char buf[BUF_SIZE];
    while (fgets(buf, BUF_SIZE, f)) {
        char *next = NULL;
        int_array_append(t->left, strtol(buf, &next, 10));
        int_array_append(t->right, strtol(next, NULL, 10));
    }
    fclose(f);
    return t;
}

int similarity(table *t) {
    int_array_bubble_sort(t->left, 0);
    int_array_bubble_sort(t->right, 0);
    size_t a_size = int_array_size(t->left);
    int total = 0;
    int left, right;
    for (size_t i = 0; i < a_size; i++) {
        int_array_get(t->left, i, &left);
        for (size_t j = 0; j < a_size; j++) {
            int_array_get(t->right, j, &right);
            if (right == left) {
                total += left;
            }
        }
    }
    return total;
}

int main(void) {
    table *t = read_file("day1.txt");
    printf("%d\n", similarity(t));
}