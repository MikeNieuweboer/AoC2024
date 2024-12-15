#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


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

int find_words(grid *g, int base_pos) {
    int occurences = 0;

    int directions[8] = {
        -1, -g->width - 1, g->width - 1,
        -g->width, g->width,
        1, -g->width + 1, g->width + 1
    };
    char word[] = "XMAS";
    for (int i = 0; i < 8; i++) {
        int current_pos = base_pos;
        for (int j = 1; j < 4; j++) {
            int new_pos = current_pos + directions[i];
            int x = current_pos % g->width;
            if ((x == 0 && i < 3) ||
                (x == g->width - 1 && i > 4) ||
                new_pos < 0 || new_pos > g->height * g->width
            ) {
                occurences--;
                break;
            }

            if (g->data[new_pos] == word[j]) {
                current_pos = new_pos;
            } else {
                occurences--;
                break;
            }
        }
        printf("\n");
        occurences++;
    }
    return occurences;
}

int *find_bases(grid *g) {
    const int max_base_count = 4000;
    int *bases = malloc(max_base_count * sizeof(int));
    if (!bases) return NULL;
    for (int i = 0; i < max_base_count; i++) {
        bases[i] = -1;
    }
    size_t total_size = g->width * g->height;
    int base_index = 0;
    for (int i = 0; i < total_size; i++) {
        printf("%c\n", g->data[i]);
        if (g->data[i] == 'X') bases[base_index++] = i;
    }
    return bases;
}

int main(void) {
    grid *g = read_grid("day4.txt");
    int *bases = find_bases(g);
    int base_index = 0;
    int base = bases[base_index++];
    int total = 0;
    while (base != -1) {
        total += find_words(g, base);
        base = bases[base_index++];
    }
    printf("%d\n", total);
    free(bases);
    free(g);
    return 0;
}
