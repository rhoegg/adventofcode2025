#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include "../common.h"
#include "main.h"
#include "heap.h"

Machine *machines;
size_t len_machines;

void parse_input(char *buf) {
    char **lines = split(buf, '\n', &len_machines);
    for (size_t i = 0; i < len_machines; i++) {
        size_t len_tokens;
        char **tokens = split(lines[i], ' ', &len_tokens);
        char *lights_token = tokens[0];
        int lights_bits = 0;
        for (char *c = lights_token + 1; *c != ']'; ++c) {
            machines[i].lights = machines[i].lights * 2 + (*c == '#');
            lights_bits++;
        }
        char *joltage_token = tokens[len_tokens - 1];
        // unused in part 1
        for (size_t j = 1; j < len_tokens - 1; ++j) {
            char *button_token = tokens[j];
            button_token[strlen(button_token) - 1] = '\0';
            size_t len_mask_tokens;
            char **mask_tokens = split(button_token + 1, ',', &len_mask_tokens);
            for (size_t k = 0; k < len_mask_tokens; ++k) {
                int b = lights_bits - atoi(mask_tokens[k]) - 1;
                machines[i].buttons[machines[i].len_buttons] += (b == 0) ? 1 : 2 << (b - 1);
            }
            machines[i].len_buttons++;
        }
        // printf("%lu buttons:", i);
        // for (int j = 0; j < machines[i].len_buttons; ++j) {
        //     printf(" %d", machines[i].buttons[j]);
        // }
        // printf("\n");
        printf("%lu has %d buttons and needs to show %d\n", i, machines[i].len_buttons, machines[i].lights);
    }
}

int main(int argc, char **argv) {
    const char *input_path = (argc > 1) ? argv[1] : "day10/sample.txt";
    if (input_path == NULL) {
        printf("input path required\n");
        exit(EXIT_FAILURE);
    }
    long len;
    char *buf = read_file(input_path, &len);

    machines = calloc(256, sizeof(Machine));
    parse_input(buf);

    part1();

    free(machines);
    free(buf);
}

void part1() {
    int part1_sum = 0;
    for (size_t i = 0; i < len_machines; ++i) {
        Machine m = machines[i];
        Trial *trials = malloc(32 * sizeof(Trial));
        // create a heap of paths, initialize with a single button press of each button, 
        TrialHeap heap = {
            .trials = trials,
            .size = 0,
            .capacity = 32
        };
        for (int j = 0; j < m.len_buttons; ++j) {
            Trial t = {
                .machine = m,
                .buttons = {m.buttons[j]},
                .len_buttons = 1
            };
            insert_heap(&heap, t);
        }

        while(heap.size > 0) {
            Trial t = extract_heap_min(&heap);
            int indicator_lights = 0;
            for (int j = 0; j < t.len_buttons; ++j) {
                indicator_lights ^= t.buttons[j]; // XOR with the next button press
            }
            // printf("%lu. Checking %d buttons:", i, t.len_buttons);
            for (int j = 0; j < t.len_buttons; ++j) {
                // printf(" %d", t.buttons[j]);
            }
            // printf(" = %d\n", indicator_lights);
            if (indicator_lights == m.lights) {
                // winner!
                part1_sum += t.len_buttons;
                // printf("machine %lu succeeded in %d button presses:", i, t.len_buttons);
                // for (int j = 0; j < t.len_buttons; ++j) {
                //     printf(" %d", t.buttons[j]);
                // }
                // printf("\n");
                break;
            } else {
                // insert all possible button presses:
                for (int j = 0; j < m.len_buttons; ++j) {
                    bool will_repeat = false;
                    for (int k = 0; k < t.len_buttons; ++k) {
                        if (m.buttons[j] == t.buttons[k]) {
                            // no need to repeat this one
                            will_repeat = true;
                        }
                    }
                    if (!will_repeat) {
                        Trial new_trial = t;
                        new_trial.buttons[new_trial.len_buttons] = m.buttons[j];
                        new_trial.len_buttons++;
                        insert_heap(&heap, new_trial);
                    }
                }
            }
        };
        free (heap.trials);
    }
    printf("Part 1: %d\n", part1_sum);
}