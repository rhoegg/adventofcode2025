#ifndef MAIN_H
#define MAIN_H

typedef struct {
    int lights;
    int buttons[32];
    int len_buttons;
} Machine;

typedef struct {
    Machine machine;
    int buttons[64];
    int len_buttons;
} Trial;

void part1();
#endif // MAIN_H