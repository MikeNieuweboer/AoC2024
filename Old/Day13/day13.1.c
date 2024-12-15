#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DataStructures/dynarr_void.h"
#include "DataStructures/stack_int.h"

#define BUF_SIZE 1024
#define MAX_STEPS 100

struct button {
    int x, y;
} typedef button;

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
    int *g = malloc(prize_pos * sizeof(int));
    if (!g) return -1;
    for (int i = 0; i < prize_pos; i++) {
        g[i] = -1;
    }
    // I should have really used a void *stack
    int_stack *depth_a = int_stack_init(100);
    if (!depth_a) return -1;
    int_stack *depth_b = int_stack_init(100);
    if (!depth_b) return -1;
    int_stack *pos_s = int_stack_init(100);
    if (!pos_s) return -1;
    int_stack *cost_s = int_stack_init(100);
    if (!cost_s) return -1;
    int_stack_append(pos_s, 0);
    int_stack_append(depth_a, 0);
    int_stack_append(depth_b, 0);
    int_stack_append(cost_s, 0);
    while (int_stack_size(pos_s) > 0) {
        int a_dep, b_dep, pos, cost;
        int_stack_pop(depth_a, &a_dep);
        int_stack_pop(depth_a, &b_dep);
        int_stack_pop(pos_s, &pos);
        int_stack_pop(cost_s, &cost);
        if (a_dep > MAX_STEPS || b_dep > MAX_STEPS) continue;
        if (g[pos] != -1 && g[pos] <= cost) continue;
        g[pos] = cost;
        int x = pos % (m->target_x + 1);
        int a_offset = m->a.x + (m->target_x + 1) * m->a.y;
        if (x + m->a.x <= m->target_x && pos + a_offset < prize_pos) {
            int_stack_append(depth_a, a_dep + 1);
            int_stack_append(depth_b, b_dep);
            int_stack_append(pos_s, pos + a_offset);
            int_stack_append(cost_s, cost + 3);
        }

        int b_offset = m->b.x + (m->target_x + 1) * m->b.y;
        if (x + m->b.x <= m->target_x && pos + b_offset < prize_pos) {
            int_stack_append(depth_a, a_dep);
            int_stack_append(depth_b, b_dep + 1);
            int_stack_append(pos_s, pos + b_offset);
            int_stack_append(cost_s, cost + 1);
        }
    }
    int_stack_free(pos_s);
    int_stack_free(cost_s);
    int_stack_free(depth_a);
    int_stack_free(depth_b);
    int total = g[prize_pos - 1];
    free(g);
    if (total < 0) {
        return 0;
    } else {
        return total;
    }
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