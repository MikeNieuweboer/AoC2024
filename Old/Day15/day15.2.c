#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "DataStructures/dynarr_int.h"

#define BUF_SIZE 1024

struct grid {
    char *data;
    size_t height, width;
    int robo_pos;
} typedef grid;


grid *read_grid(char *file_name) {
    if (!file_name) return NULL;

    grid *g = (grid *)malloc(sizeof(grid));
    if (!g) return NULL;

    FILE *f = fopen(file_name, "r");
    fseek(f, 0, SEEK_END);
    size_t total_size = (unsigned long)ftell(f) * 2;
    g->data = (char *)malloc(total_size * sizeof(char));
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
        if (!i) g->width = 2 * (len - 1);
        for (int j = 0; j < len; j++) {
            if (buf[j] != '\n') {
                switch (buf[j]) {
                    case '#':
                        g->data[i++] = '#';
                        g->data[i++] = '#';
                        break;
                    case '.':
                        g->data[i++] = '.';
                        g->data[i++] = '.';
                        break;
                    case 'O':
                        g->data[i++] = '[';
                        g->data[i++] = ']';
                        break;
                    case '@':
                        g->robo_pos = i;
                        g->data[i++] = '@';
                        g->data[i++] = '.';
                        break;
                }
            }
        }
        height++;
    }
    g->height = height;
    fclose(f);
    return g;
}

int char_to_move(char c) {
    switch (c) {
        case '<':
            return 0;
        case '^':
            return 1;
        case '>':
            return 2;
        case 'v':
            return 3;
        default:
            return -1;
    }
}

int_array *read_moves(char *file_name) {
    if (!file_name) return NULL;
    FILE *f = fopen(file_name, "r");
    int_array *a = int_array_init(128);
    if (!a) {
        fclose(f);
        return NULL;
    }
    char buf[BUF_SIZE];
    while (fgets(buf, BUF_SIZE, f)) {
        for (int i = 0; buf[i] != '\0'; i++) {
            int c = char_to_move(buf[i]);
            if (c != -1) int_array_append(a, c);
        }
    }
    fclose(f);
    return a;
}

int can_push_vert(grid *g, int pos, int direction) {
    int new_pos = pos + direction;
    switch (g->data[new_pos]) {
        case '#':
            return 1;
        case '[':
            return can_push_vert(g, new_pos, direction) ||
                    can_push_vert(g, new_pos + 1, direction);
        case ']':
            if (g->data[pos] == ']') {
                return 0;
            }
            return can_push_vert(g, new_pos, direction) ||
                    can_push_vert(g, new_pos - 1, direction);
        case '.':
            return 0;
        default:
            return 0;
    }
}

void print_grid(grid *g) {
    for (int r = 0; r < g->height; r++) {
        fwrite(g->data + r * g->width, sizeof(char), g->width, stdout);
        printf("\n");
    }
}

void push_cells(grid *g, int pos, int direction) {
    int new_pos = pos + direction;
    switch (g->data[new_pos]) {
        case '[':
            push_cells(g, new_pos, direction);
            push_cells(g, new_pos + 1, direction);
            break;
        case ']':
            push_cells(g, new_pos, direction);
            push_cells(g, new_pos - 1, direction);
            break;
    }
    g->data[new_pos] = g->data[pos];
    g->data[pos] = '.';
}

int single_move(grid *g, int move) {
    if (!g) return 1;
    int moves[4] = { -1, -g->width, 1, g->width };
    move = moves[move];
    // Horizontal
    if (abs(move) <= 1) {
        int end_pos = g->robo_pos + move;
        while (g->data[end_pos] == '[' || g->data[end_pos] == ']') {
            end_pos += move;
        }
        if (g->data[end_pos] == '#') return 0;
        while (g->data[end_pos - move] != '@') {
            g->data[end_pos] = g->data[end_pos - move];
            end_pos -= move;
        }
        g->data[end_pos] = '@';
        g->data[g->robo_pos] = '.';
        g->robo_pos += move;
    } else {
        // Vertical
        int cant_push = can_push_vert(g, g->robo_pos, move);
        if (!cant_push) {
            push_cells(g, g->robo_pos, move);
            g->robo_pos += move;
        }
    }
    return 0;
}

int all_moves(grid *g, int_array *a) {
    int array_size = int_array_size(a);
    for (int i = 0; i < array_size; i++) {
        int move;
        int_array_get(a, i, &move);
        single_move(g, move);
    }
    return 0;
}

int count_points(grid *g) {
    int index = 0;
    int total = 0;
    for (int r = 0; r < g->height; r++) {
        for (int c = 0; c < g->width; c++) {
            char cell = g->data[index++];
            if (cell == '[') {
                total += r * 100 + c;
            }
        }
    }
    return total;
}

int main(void) {
    grid *g = read_grid("day15.maze.txt");
    int_array *a = read_moves("day15.moves.txt");
    all_moves(g, a);
    print_grid(g);
    printf("%d\n", count_points(g));
    return 0;
}