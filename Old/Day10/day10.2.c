#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "DataStructures/stack_int.h"

#define BUF_SIZE 1024

struct grid {
    int *data;
    size_t height, width;
} typedef grid;

grid *read_grid(char *file_name) {
    if (!file_name) return NULL;

    grid *g = (grid *)malloc(sizeof(grid));
    if (!g) return NULL;

    FILE *f = fopen(file_name, "r");
    fseek(f, 0, SEEK_END);
    size_t total_size = (unsigned long)ftell(f);

    g->data = (int *)malloc(total_size * sizeof(int));
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
            if (isdigit(buf[j])) g->data[i++] = (int)(buf[j] - '0');
        }
        height++;
    }
    g->height = height;
    fclose(f);
    return g;
}

int find_peaks(grid *g, int base_pos) {
    int peak_pos[200] = { -1 };
    int peaks = 0;

    int_stack *s = int_stack_init(1024);
    int_stack_append(s, base_pos);
    int directions[4] = { -1, -g->width, 1, g->width };
    while (int_stack_size(s) > 0) {
        int current_pos;
        int_stack_pop(s, &current_pos);
        int height = g->data[current_pos];
        if (height == 9) {
            peaks++;
            continue;
        }
        for (int i = 0; i < 4; i++) {
            int new_pos = current_pos + directions[i];
            int x = current_pos % g->width;
            if ((x == 0 && directions[i] == -1) ||
                (x == g->width - 1 && directions[i] == 1) ||
                new_pos < 0 || new_pos > g->height * g->width
            ) {
                continue;
            }
            if (g->data[new_pos] == height + 1) {
                int_stack_append(s, new_pos);
            }
        }
    }
    return peaks;
}

int *find_bases(grid *g) {
    const int max_base_count = 250;
    int *bases = malloc(max_base_count * sizeof(int));
    if (!bases) return NULL;
    for (int i = 0; i < max_base_count; i++) {
        bases[i] = -1;
    }
    size_t total_size = g->width * g->height;
    int base_index = 0;
    for (int i = 0; i < total_size; i++) {
        if (g->data[i] == 0) bases[base_index++] = i;
    }
    return bases;
}

int main(void) {
    grid *g = read_grid("day10.1.txt");
    int *bases = find_bases(g);
    int base_index = 0;
    int base = bases[base_index++];
    int total = 0;
    while (base != -1) {
        total += find_peaks(g, base);
        base = bases[base_index++];
    }
    printf("%d\n", total);
    free(bases);
    free(g);
    return 0;
}
