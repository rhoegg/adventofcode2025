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
    int buttons[32];
    int counts[32];
    int len_buttons;
} Trial;

void part1();
void part2();
int compare_int(const void *a, const void *b);
int find_button_presses(Machine *machine);
bool button_influences_counter(Machine *m, int button_index, int counter);
#endif // MAIN_H