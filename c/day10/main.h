#ifndef MAIN_H
#define MAIN_H

typedef struct {
    int lights;
    int buttons[32];
    int len_buttons;
    int joltage[32];
    int len_joltage;
} Machine;

typedef struct {
    Machine machine;
    int buttons[64];
    int len_buttons;
} Trial;

void part1();
void part2();
int compare_int(const void *a, const void *b);
#endif // MAIN_H