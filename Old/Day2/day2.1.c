#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 1024
#define REPORT_COUNT 1000
#define MAX_NUM 8

struct report {
    int *num;
    size_t size;
} typedef report;

struct binder {
    report **reports;
    size_t size;
} typedef binder;

report *create_report(char *line) {
    if (!line) return NULL;
    char *current = NULL;
    char *next = line;
    report *r = (report *)malloc(sizeof(report));
    r->num = malloc(MAX_NUM * sizeof(int));
    r->size = 0;
    while (1) {
        current = next;
        int num = strtol(current, &next, 10);
        if (current == next) break;
        r->num[r->size++] = num;
    }
    return r;
}

binder *gather_reports(char *file_name) {
    if (!file_name) return NULL;

    binder *b = (binder *)malloc(sizeof(binder));
    if (!b) return NULL;
    b->size = 0;
    b->reports = (report **)malloc(REPORT_COUNT * sizeof(report *));
    if (!b->reports) {
        free(b);
        return NULL;
    }

    FILE *f = fopen(file_name, "r");
    char buf[BUF_SIZE];
    while (fgets(buf, BUF_SIZE, f)) {
        b->reports[b->size++] = create_report(buf);
    }
    return b;
}

int grade_reports(binder *b) {
    if (!b) return -1;
    int correct = b->size;
    for (int i = 0; i < b->size; i++) {
        report *r = b->reports[i];
        int failure = 0;
        for (int ascending = 0; ascending <= 1; ascending++) {
            int jkog = 1;
            for (int j = 0; j < r->size - 1; j++) {
                int dif = r->num[j + 1] - r->num[j];
                if (abs(dif) < 1 || abs(dif) > 3 ||
                    (ascending && dif < 0) ||
                    (!ascending && dif > 0)
                ) {
                    if (!jkog-- ||
                        (((j < r->size - 2 && (abs(r->num[j + 2] - r->num[j]) < 1 ||
                        abs(r->num[j + 2] - r->num[j]) > 3 ||
                        (ascending && r->num[j + 2] - r->num[j] < 0) ||
                        (!ascending && r->num[j + 2] - r->num[j] > 0)))
                        ||
                        (j > 0 && (abs(r->num[j - 1] - r->num[j]) < 1 ||
                        abs(r->num[j - 1] - r->num[j]) > 3 ||
                        (ascending && r->num[j] - r->num[j - 1] < 0) ||
                        (!ascending && r->num[j] - r->num[j - 1] > 0))))
                        &&
                        ((j < r->size - 2 && (abs(r->num[j + 2] - r->num[j + 1]) < 1 ||
                        abs(r->num[j + 2] - r->num[j + 1]) > 3 ||
                        (ascending && r->num[j + 2] - r->num[j + 1] < 0) ||
                        (!ascending && r->num[j + 2] - r->num[j + 1] > 0)))
                        ||
                        (j > 0 && (abs(r->num[j - 1] - r->num[j + 1]) < 1 ||
                        abs(r->num[j - 1] - r->num[j + 1]) > 3 ||
                        (ascending && r->num[j + 1] - r->num[j - 1] < 0) ||
                        (!ascending && r->num[j + 1] - r->num[j - 1] > 0))))
                    )) {
                        if (failure++) {
                            correct--;
                        }
                        break;
                    }
                    if (((j < r->size - 2 && (abs(r->num[j + 2] - r->num[j + 1]) < 1 ||
                        abs(r->num[j + 2] - r->num[j + 1]) > 3 ||
                        (ascending && r->num[j + 2] - r->num[j + 1] < 0) ||
                        (!ascending && r->num[j + 2] - r->num[j + 1] > 0)))
                        ||
                        (j > 0 && (abs(r->num[j - 1] - r->num[j + 1]) < 1 ||
                        abs(r->num[j - 1] - r->num[j + 1]) > 3 ||
                        (ascending && r->num[j + 1] - r->num[j - 1] < 0) ||
                        (!ascending && r->num[j + 1] - r->num[j - 1] > 0))))) {
                        r->num[j + 1] = r->num[j];
                    }
                }
            }
        }
    }
    return correct;
}

int main(void) {
    binder *b = gather_reports("day2.txt");
    int correct = grade_reports(b);
    printf("%d\n", correct);
}