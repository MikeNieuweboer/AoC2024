#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "DataStructures/stack_int.h"

#define BUF_SIZE 1024

struct grid {
    char *data;
    size_t height, width;
} typedef grid;

grid *read_grid(char *file_name) {
    if (!file_name) return NULL;

    grid *g = (grid *)malloc(sizeof(grid));
    if (!g) return NULL;

    FILE *f = fopen(file_name, "r");
    fseek(f, 0, SEEK_END);
    size_t total_size = (unsigned long)ftell(f);

    g->data = (char *)malloc(total_size * sizeof(int));
    if (!g->data) {
        fclose(f);
        free(g);
        return NULL;
    }

    fseek(f, 0, SEEK_SET);
    char buf[BUF_SIZE];
    size_t i = 0;
    int height = 0;
    while (fgets(buf, BUF_SIZE, f)) {
        int len = strlen(buf);
        if (!i) g->width = len - 1;
        for (int j = 0; j < len; j++) {
            if (isalpha(buf[j])) g->data[i++] = buf[j];
        }
        height++;
    }
    g->height = height;
    fclose(f);
    return g;
}

int single_fence_cost(grid *g, int *visited, int pos) {
    int fences = 0;
    int area = 0;
    char id = g->data[pos];
    int directions[4] = { -1, 1, g->width, -g->width };

    int_stack *s = int_stack_init(1024);
    int_stack_append(s, pos);
    visited[pos] = 1;

    while (int_stack_size(s) > 0) {
        area++;
        int current_pos;
        int_stack_pop(s, &current_pos);
        for (int i = 0; i < 4; i++) {
            int new_pos = current_pos + directions[i];
            int x = current_pos % g->width;
            if ((x == 0 && directions[i] == -1) ||
                (x == g->width - 1 && directions[i] == 1) ||
                new_pos < 0 || new_pos > g->height * g->width
            ) {
                if (
                    (i < 2 && (
                    current_pos < g->width ||
                    g->data[current_pos - g->width] != id
                    )) ||
                    (i >= 2 && (
                    x == 0 ||
                    g->data[current_pos - 1] != id
                    ))
                ) {
                    fences++;
                }
                continue;
            }
            if (g->data[new_pos] == id) {
                if (!visited[new_pos]) {
                    int_stack_append(s, new_pos);
                    visited[new_pos] = 1;
                }
            } else {
                if (
                    (i < 2 && (
                    current_pos < g->width ||
                    g->data[current_pos - g->width] != id ||
                    g->data[new_pos - g->width] == id
                    )) ||
                    (i >= 2 && (
                    x == 0 ||
                    g->data[current_pos - 1] != id ||
                    g->data[new_pos - 1] == id
                    ))
                ) {
                    fences++;
                }
            }
        }
    }
    printf("%c, %d, %d\n", id, area, fences);
    return area * fences;
}

int total_fence_cost(grid *g) {
    if (!g) return -1;
    int total = 0;
    int *visited = (int *)calloc(sizeof(int), g->width * g->height);
    if (!visited) return -1;
    for (int i = 0; i < g->height * g->width; i++) {
        if (visited[i] == 1) continue;
        total += single_fence_cost(g, visited, i);
    }
    return total;
}

int main(void) {
    grid *g = read_grid("day12.txt");
    int cost = total_fence_cost(g);
    printf("Cost: %d\n", cost);
    free(g);
    return 0;
}
