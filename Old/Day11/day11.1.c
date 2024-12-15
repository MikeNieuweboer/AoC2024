/*
 * C is very cool
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUF_SIZE 1024

typedef struct stone stone;

struct master_stone {
    stone *first;
    long size;
} typedef master_stone;

struct stone {
    long num;
    long factor;
    stone *next;
} typedef stone;

void break_stones(master_stone *ms) {
    if (!ms) return;
    stone *s = ms->first;
    free(ms);
    while (s) {
        stone *brittle_s = s->next;
        free(s);
        s = brittle_s;
    }
}

void stop_blinking(master_stone *ms) {
    if (!ms) return;
    stone *s = ms->first;
    while (s) {
        printf("( %ld ) ", s->num);
        s = s->next;
    }
    printf("\n");
}

master_stone *read_stones(char *file_name) {
    FILE *f = fopen(file_name, "r");
    char buf[BUF_SIZE];
    if (!fgets(buf, BUF_SIZE, f)) return NULL;
    char *current = NULL;
    char *next = buf;
    master_stone *ms = (master_stone *)malloc(sizeof(master_stone));
    if (!ms) return NULL;
    ms->size = 0;
    stone *prev_s = NULL;
    while (1) {
        current = next;
        long num = strtol(current, &next, 10);
        if (current == next) break;
        stone *s = malloc(sizeof(stone));
        if (!s) {
            break_stones(ms);
            return NULL;
        }
        s->next = NULL;
        s->num = num;
        s->factor = 1;
        if (!prev_s) {
            ms->first = s;
        } else {
            prev_s->next = s;
        }
        ms->size++;
        prev_s = s;
    }
    fclose(f);
    return ms;
}

int stone_width(stone *s) {
    if (!s) return -1;
    long num = s->num;
    int width = 0;
    while (num) {
        width++;
        num /= 10;
    }
    return width;
}

int split_stones(stone *s, int width) {
    if (width < 2) return 1;
    int split_point = width / 2 - 1;
    long split_num = 10;
    while (split_point--) {
        split_num *= 10;
    }
    long num = s->num;
    long left_num = num / split_num;
    long right_num = num % split_num;
    stone *right_s = (stone *)malloc(sizeof(stone));
    if (!right_s) return 1;
    right_s->num = right_num;
    right_s->next = s->next;
    right_s->factor = s->factor;
    s->num = left_num;
    s->next = right_s;
}

// Massive outer wilds spoilers!!!
int blink(master_stone *ms) {
    if (!ms) return 1;
    stone *s = ms->first;
    while (s) {
        stone *next_s = s->next;
        int width = stone_width(s);
        if (s->num == 0) {
            s->num = 1;
        } else if (width % 2 == 0) {
            split_stones(s, width);
            ms->size += s->factor;
        } else {
            s->num = s->num * 2024;
        }
        s = next_s;
    }
}

int compact_stones(master_stone *ms) {
    if (!ms) return 1;
    stone *s = ms->first;
    while (s) {
        stone *prev_brittle_s = s;
        stone *brittle_s = s->next;
        while (brittle_s) {
            if (brittle_s->num == s->num) {
                prev_brittle_s->next = brittle_s->next;
                s->factor += brittle_s->factor;
                free(brittle_s);
                brittle_s = brittle_s->next;
                continue;
            }
            prev_brittle_s = brittle_s;
            brittle_s = brittle_s->next;
        }
        s = s->next;
    }
}

int main(void) {
    master_stone *ms = read_stones("day11.txt");
    if (!ms) return 1;
    for (int i = 0; i < 75; i++) {
        printf("%d\n", i);
        blink(ms);
        compact_stones(ms);
    }
    printf("%ld\n", ms->size);
    break_stones(ms);
    return 0;
}
