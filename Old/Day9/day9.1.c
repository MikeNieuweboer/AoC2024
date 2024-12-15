#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUF_SIZE 100000
#define EMPTY -1
#define END -2

enum direction { LEFT, RIGHT };

int *create_disk(char *buf) {
    size_t total_size = 1;
    size_t index = 0;
    char c = buf[index];

    while (c != '\n') {
        total_size += c - '0';
        c = buf[++index];
    }
    int *disk = (int *)malloc(total_size * sizeof(int));
    if (!disk) return NULL;

    index = 0;
    size_t disk_index = 0;
    int id = 0;
    c = buf[index];
    int even = 1;
    while (c) {
        for (int i = 0; i < (c - '0'); i++) {
            disk[disk_index + i] = even ? id : EMPTY;
        }
        disk_index += c - '0';
        even = even ^ 1;
        id += even;
        c = buf[++index];
    }
    disk[total_size - 1] = END;
    return disk;
}


int next_non_empty(int *disk, int current, enum direction direction) {
    if (!disk) return -1;
    int step_size = direction == RIGHT ? 1 : -1;
    current += step_size;
    while (current >= 0) {
        int id = disk[current];
        if (id >= 0) return current;
        if (id == END) return -1;
        current += step_size;
    }
    return -1;
}

int sort_disk(int *disk) {
    if (!disk) return 1;
    int start = 0;
    int end = -1;
    while (disk[++end] != END);
    end--;
    while (start < end) {
        for (; start < end; start++) {
            if (disk[start] == EMPTY) {
                disk[start] = disk[end];
                disk[end] = EMPTY;
                break;
            }
        }
        end = next_non_empty(disk, end, LEFT);
        if (end < 0) break;
    }
    return 0;
}

static void print_disk(int *disk) {
    if (!disk) {
        fprintf(stderr, "No disk to print\n");
        return;
    }
    int id = disk[0];
    for (int i = 1; id != END; i++) {
        printf("%c", id >= 0 ? id + '0' : '.');
        id = disk[i];
    }
    printf("\n");
}

static unsigned long calculate_checksum(int *disk) {
    if (!disk) return -1;
    int index = 0;
    int id = disk[index];
    unsigned long checksum = 0;
    while (id != END && id != EMPTY) {
        checksum += id * index;
        id = disk[++index];
    }
    return checksum;
}

int main(void) {
    FILE *f = fopen("day9input1.txt", "r");
    // FILE *f = fopen("day9test.txt", "r");
    char buf[BUF_SIZE];
    if (!fgets(buf, BUF_SIZE, f)) {
        return 1;
    }
    int *disk = create_disk(buf);
    if (!disk) return 1;
    fclose(f);

    if (sort_disk(disk)) return 1;
    long unsigned checksum = calculate_checksum(disk);
    printf("%lu\n", checksum);
    return 0;
}