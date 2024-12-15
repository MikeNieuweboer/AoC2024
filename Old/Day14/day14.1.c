#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1024
#define WIDTH 101
#define HEIGHT 103
#define TIME_STEPS 1

struct vec2 {
    int x, y;
} typedef vec2;

typedef struct robot robot;

struct robot {
    vec2 pos;
    vec2 vel;
    robot *next;
};

robot *read_robot(char *line) {
    if (!line) return NULL;
    robot *r = (robot *)malloc(sizeof(robot));
    if (!r) return NULL;
    int x_pos = strtol(line + 2, &line, 10);
    int y_pos = strtol(line + 1, &line, 10);
    int x_vel = strtol(line + 3, &line, 10);
    int y_vel = strtol(line + 1, &line, 10);
    r->pos = (vec2){ x_pos, y_pos };
    r->vel = (vec2){ x_vel, y_vel };
    r->next = NULL;
    return r;
}

robot *read_file(char *file_name) {
    if (!file_name) return NULL;
    FILE *f = fopen(file_name, "r");
    char buf[BUF_SIZE];
    robot *r_first = NULL;
    robot *r_curr;
    while(fgets(buf, BUF_SIZE, f)) {
        robot *r = read_robot(buf);
        if (!r) return NULL;
        if (!r_first) {
            r_first = r;
        } else {
            r_curr->next = r;
        }
        r_curr = r;
    }
    return r_first;
}

void move_robots(robot *r) {
    while (r) {
        robot *r_tmp = r->next;
        int new_x = (r->pos.x + TIME_STEPS * r->vel.x) % WIDTH;
        new_x = new_x < 0 ? new_x + WIDTH : new_x;
        int new_y = (r->pos.y + TIME_STEPS * r->vel.y) % HEIGHT;
        new_y = new_y < 0 ? new_y + HEIGHT : new_y;
        r->pos = (vec2){ new_x, new_y };
        r = r_tmp;
    }
}

int security_level(robot *r) {
    int scores[4] = { 0, 0, 0, 0 };
    while (r) {
        robot *r_tmp = r->next;
        if (r->pos.x != WIDTH / 2 && r->pos.y != HEIGHT / 2) {
            int index = (r->pos.x > (WIDTH / 2)) + 2 * (r->pos.y > (HEIGHT / 2));
            scores[index]++;
        }
        r = r_tmp;
    }
    return scores[0] * scores[1] * scores[2] * scores[3];
}

void robot_revolution(robot *r) {
    while (r) {
        robot *r_tmp = r->next;
        free(r);
        r = r_tmp;
    }
}

void create_pbm(robot *r, int index) {
    char buf[BUF_SIZE];
    sprintf(buf, "Trees/tree%d.pbm", index);
    FILE *f = fopen(buf, "w");
    fwrite("P1\n101 103\n", sizeof(char), 11, f);
    char *image = (char *)malloc(WIDTH * HEIGHT * sizeof(char));
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        image[i] = '0';
    }
    while (r) {
        robot *r_tmp = r->next;
        image[r->pos.x + r->pos.y * WIDTH] = '1';
        r = r_tmp;
    }

    for (int r = 0; r < HEIGHT; r++) {
        fwrite(image + r * WIDTH, sizeof(char), WIDTH, f);
        fputc('\n', f);
    }
    fclose(f);
}

int main(void) {
    robot *robots = read_file("day14.txt");
    for (int i = 0; i < 100000; i++) {
        create_pbm(robots, i);
        move_robots(robots);
    }
    int sec_lev = security_level(robots);
    printf("%d\n", sec_lev);
    robot_revolution(robots);
    return 0;
}