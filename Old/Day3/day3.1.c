#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum find_state { FIND_MUL, FIND_FIRST_NUM, FOUND_FIRST_NUM, FIND_SECOND_NUM, FOUND_SECOND_NUM };

char *read_file(char *file_name) {
    if (!file_name) return NULL;
    FILE *f = fopen(file_name, "r");
    fseek(f, 0, SEEK_END);
    int length = ftell(f);
    char *input = (char *)malloc((length + 1) * sizeof(char));
    if (!input) return NULL;
    fseek(f, 0, SEEK_SET);
    fread(input, 1, length, f);
    input[length] = '\0';
    fclose(f);
    return input;
}

int next_mul_result(char **input) {
    if (!input || !*input) return -1;

    int first, second;
    while (**input != '\0') {
        *input = strstr(*input, "mul(");
        if (!*input) return -1;
        *input = *input + 4;
        if (!isdigit(**input)) continue;
        first = strtol(*input, input, 10);
        if (**input != ',') continue;
        *input = *input + 1;
        if (!isdigit(**input)) continue;
        second = strtol(*input, input, 10);
        if (**input != ')') continue;
        return first * second;
    }
    return -1;
}

int do_dont_do_it(char *input) {
    if (!input) return 1;
    char *new_input = (char *)malloc(strlen(input) * sizeof(char));
    if (!new_input) return -1;
    char *current = input;
    while (1) {
        current = strstr(current, "don't()");
        if (!current) break;
        char *next = strstr(current, "do()");
        while (current != next && *current != '\0') {
            *current = ' ';
            current += 1;
        }
    }
    return 0;
}

int main(void) {
    char *input = read_file("day3.txt");
    do_dont_do_it(input);
    int total = 0;
    while (1) {
        int res = next_mul_result(&input);
        if (res == -1) break;
        total += res;
    }
    printf("total: %d\n", total);
    return 0;
}