#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DataStructures/dynarr_void.h"

#define BUF_SIZE 1024
#define OFFSET 10000000000000
// #define OFFSET 0

struct button {
    int x, y;
} typedef button;

struct position {
    long x, y;
} typedef position;

struct machine {
    button a, b;
    unsigned long target_x, target_y;
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
    m->target_x = OFFSET + strtol(line + 2, NULL, 10);
    line = strstr(line, "Y=");
    if (!line) return 1;
    m->target_y = OFFSET + strtol(line + 2, NULL, 10);
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

// res = [[a_x, b_x], [a_y, b_y]] [n, m]
//
// 1/d * [[b_y, -b_x], [-a_y, a_x]] res = [n, m]
unsigned long solve_machine(machine *m) {
    int a_x = m->a.x;
    int a_y = m->a.y;
    int b_x = m->b.x;
    int b_y = m->b.y;
    unsigned long det = labs(a_x * b_y - b_x * a_y);
    unsigned long res_x = labs(b_y * m->target_x - b_x * m->target_y);
    unsigned long res_y = labs(-a_y * m->target_x + a_x * m->target_y);
    if ((res_x % det) == 0 && (res_y % det) == 0) {
        unsigned long n = res_x / det;
        unsigned long k = res_y / det;
        return n * 3 + k;
    }
    return 0;
}

unsigned long solve_arcade(void_array *a) {
    if (!a) return -1;
    unsigned long total = 0;
    while (void_array_size(a) > 0) {
        void *vm;
        void_array_pop(a, &vm);
        machine *m = (machine *)vm;
        unsigned long res = solve_machine(m);
        if (res < 0) return -1;
        total += res;
        free(m);
    }
    return total;
}

int main(void) {
    void *a = read_file("day13.txt");
    unsigned long total = solve_arcade(a);
    printf("%lu\n", total);
    void_array_free(a);
    return 0;
}