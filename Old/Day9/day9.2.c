#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUF_SIZE 100000
#define EMPTY -1
#define END -2

enum direction { LEFT, RIGHT };

struct file_block {
    int begin, end;
    int id;
};

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


struct file_block *next_block(
    int *disk, int current,
    enum direction direction, int empty
) {
    if (!disk) return NULL;
    int step_size = direction == RIGHT ? 1 : -1;
    while (current >= 0) {
        int id = disk[current];
        if (id == END) return NULL;
        if (!empty == id >= 0) {
            struct file_block *f = (struct file_block *)malloc(sizeof(struct file_block));
            if (!f) return NULL;
            f->id = id;
            if (direction == RIGHT) {
                f->begin = current;
            } else {
                f->end = current;
            }
            current += step_size;
            while (current >= 0 && disk[current] == id) current += step_size;
            if (direction == RIGHT) {
                f->end = current - 1;
            } else {
                f->begin = current + 1;
            }
            return f;
        }
        current += step_size;
    }
    return NULL;
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

int sort_disk(int *disk) {
    if (!disk) return 1;
    int end = -1;
    while (disk[++end] != END);
    end--;
    while (end >= 0) {
        struct file_block *f = next_block(disk, end, LEFT, 0);
        end = f->begin - 1;
        int file_size = f->end - f->begin;
        struct file_block *empty = next_block(disk, 0, RIGHT, 1);
        while (empty && empty->end < f->begin) {
            if (empty->end - empty->begin >= file_size) {
                for (int i = 0; i <= file_size; i++) {
                    disk[empty->begin + i] = disk[f->begin + i];
                    disk[f->begin + i] = EMPTY;
                }
                break;
            }
            empty = next_block(disk, empty->end + 1, RIGHT, 1);
        }
    }
    // print_disk(disk);
    return 0;
}

static unsigned long calculate_checksum(int *disk) {
    if (!disk) return -1;
    int index = 0;
    int id = disk[index];
    unsigned long checksum = 0;
    while (id != END) {
        if (id != EMPTY) {
            checksum += id * index;
        }
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
    // print_disk(disk);

    if (sort_disk(disk)) return 1;
    long unsigned checksum = calculate_checksum(disk);
    printf("%lu\n", checksum);
    return 0;
}