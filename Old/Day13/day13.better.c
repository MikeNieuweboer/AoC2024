#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DataStructures/dynarr_void.h"

#define BUF_SIZE 1024
#define MAX_STEPS 100

struct button {
    int x, y;
} typedef button;

struct position {
    int x, y;
} typedef position;

struct machine {
    button a, b;
    int target_x, target_y;
} typedef machine;

button read_button(char *line) {
    button b = { x: -1, y: -1 };
    if (!line) return b;
    line = strstr(line, "X+");
    if (!line) return b;
    b.x = strtol(line + 2, NULL, 10);
    line = strstr(line, "Y+");
    if (!line) return b;
    b.y = strtol(line + 2, NULL, 10);
    return b;
}

int read_prize(machine *m, char *line) {
    if (!line | !m) return 1;
    line = strstr(line, "X=");
    if (!line) return 1;
    m->target_x = strtol(line + 2, NULL, 10);
    line = strstr(line, "Y=");
    if (!line) return 1;
    m->target_y = strtol(line + 2, NULL, 10);
    return 0;
}

void_array *read_file(char *file_name) {
    if (!file_name) return NULL;

    void_array *a = void_array_init(10);
    if (!a) return NULL;
    FILE *f = fopen(file_name, "r");
    char buf[BUF_SIZE];
    while (fgets(buf, BUF_SIZE, f)) {
        machine *m = (machine *)malloc(sizeof(machine));
        if (!m) return NULL;
        m->a = read_button(buf);
        if (!fgets(buf, BUF_SIZE, f)) return NULL;
        m->b = read_button(buf);
        if (!fgets(buf, BUF_SIZE, f)) return NULL;
        if (read_prize(m, buf)) return NULL;
        fseek(f, 1, SEEK_CUR);
        void_array_append(a, (void *)m);
    }
    fclose(f);
    return a;
}

void print_machines(void_array *a) {
    if (!a) return;
    int i = 0;
    while (void_array_size(a) > i) {
        void *vm;
        void_array_get(a, i++, &vm);
        machine *m = (machine *)vm;
        printf("a: %d, %d\nb: %d, %d\nprize: %d, %d\n\n", m->a.x, m->a.y, m->b.x, m->b.y, m->target_x, m->target_y);
    }
}

int solve_machine(machine *m) {
    if (!m) return -1;
    int prize_pos = (m->target_x + 1) * (m->target_y + 1);
    int total_size = MAX_STEPS * MAX_STEPS;
    position *g = calloc(MAX_STEPS * MAX_STEPS, sizeof(position));
    if (!g) return -1;
    int min_cost = -1;
    int i = 1;
    int a_x = m->a.x;
    int a_y = m->a.y;
    int b_x = m->b.x;
    int b_y = m->b.y;
    int illegal = 0;
    for (int b = 1; b < MAX_STEPS; b++) {
        int new_x = b_x + g[i - 1].x;
        int new_y = b_y + g[i - 1].y;
        if (new_x < m->target_x && new_y < m->target_y) {
            g[i].x = new_x;
            g[i++].y = new_y;
        } else {
            if (new_x == m->target_x && new_y == m->target_y) min_cost = b;
            g[i].x = -1;
            break;
        }
    }
    for (int a = 1; a < MAX_STEPS; a++) {
        i = a * MAX_STEPS;
        for (int b = 0; b < MAX_STEPS; b++) {
            if (g[i - MAX_STEPS].x == -1) {
                g[i].x = -1;
                break;
            }
            int new_x = a_x + g[i - MAX_STEPS].x;
            int new_y = a_y + g[i - MAX_STEPS].y;
            if (new_x < m->target_x && new_y < m->target_y) {
                g[i].x = new_x;
                g[i++].y = new_y;
            } else {
                if (new_x == m->target_x && new_y == m->target_y) {
                    int new_cost = b + 3 * a;
                    min_cost = new_cost < min_cost || min_cost == -1 ? new_cost : min_cost;
                }
                g[i].x = -1;
                break;
            }
        }
    }
    free(g);
    return min_cost < 0 ? 0 : min_cost;
}

int solve_arcade(void_array *a) {
    if (!a) return -1;
    int total = 0;
    while (void_array_size(a) > 0) {
        void *vm;
        void_array_pop(a, &vm);
        machine *m = (machine *)vm;
        int res = solve_machine(m);
        if (res < 0) return -1;
        total += res;
        free(m);
    }
    return total;
}

int main(void) {
    void *a = read_file("day13.txt");
    int total = solve_arcade(a);
    printf("%d\n", total);
    void_array_free(a);
    return 0;
}