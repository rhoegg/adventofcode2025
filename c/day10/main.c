#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include <assert.h>
#include "../common.h"
#include "main.h"
#include "heap.h"
#include "spookyhash.h"

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
        joltage_token++;
        joltage_token[strlen(joltage_token) - 1] = '\0';
        printf("%lu has %d buttons and needs to show %d with joltage %s\n", i, machines[i].len_buttons, machines[i].lights, joltage_token);
        size_t joltage_count;
        char **joltage_tokens = split(joltage_token, ',', &joltage_count); // we already have the count via lights_bits but why not
        machines[i].len_joltage = (int) joltage_count;
        for (int j = 0; j < machines[i].len_joltage; ++j) {
            machines[i].joltage[j] = atoi(joltage_tokens[j]);
        }
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
    part2();

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
            // for (int j = 0; j < t.len_buttons; ++j) {
                // printf(" %d", t.buttons[j]);
            // }
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

void part2() {
    int part2_sum = 0;
    const uint64_t hashseed = 464646464646; 

    for (size_t i = 0; i < len_machines; ++i) {
        Machine m = machines[i];
        Trial *trials = malloc(32 * sizeof(Trial));
        uint64_t *seen_buttons = malloc(8192 * sizeof(uint64_t));
        int len_seen_buttons = 0;
        int capacity_seen_buttons = 8192;
        long seen_count = 0;

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

        printf("Starting machine %lu\n", i);
        long combinations_tried = 0;
        while(heap.size > 0) {
            Trial t = extract_heap_min(&heap);
            int joltages[32];
            for (int j = 0; j < t.machine.len_joltage; ++j) {
                joltages[j] = 0;
            }
            for (int j = 0; j < t.len_buttons; ++j) {
                // for this button, increment the proper counters
                int button_value = t.buttons[j];
                for (int k = 0; k < m.len_joltage; ++k) {
                    if (button_value % 2 == 1) {
                        joltages[m.len_joltage - k - 1]++; // increment the joltage for this position on this button
                    }
                    button_value /= 2;
                }
                // for (int k = 0; k < m.len_joltage; ++k) {
                //     printf("%d ", joltages[k]);
                // }
                // printf("\n");
            }

            if (!memcmp(m.joltage, joltages, m.len_joltage * sizeof(int))) { // arrays are equal
                // winner!
                part2_sum += t.len_buttons;
                printf("machine %lu succeeded in %d button presses:", i, t.len_buttons);
                for (int j = 0; j < t.len_buttons; ++j) {
                    printf(" %d", t.buttons[j]);
                }
                printf("\n");
                break;
            } else {
                combinations_tried++;
                if (combinations_tried % 1000 == 0) {
                    printf("Tried %ld combinations, button length is %d, current joltage:", combinations_tried, t.len_buttons);
                    for (int j = 0; j < m.len_joltage; ++j) {
                        printf(" %d", joltages[j]);
                    }
                    printf("\n");
                }
                // if any joltages are too high, this base set of button presses is invalid!
                bool too_much_joltage = false;
                for (int j = 0; j < m.len_joltage; ++j) {
                    if (joltages[j] > m.joltage[j]) {
                        // joltage overload!
                        printf("pruning a combination because too much joltage was found!\n");
                        too_much_joltage = true;
                    }
                }
                if (!too_much_joltage) {
                    for (int j = 0; j < m.len_buttons; ++j) {
                        // for this one repeats are desirable but we want combinations not permutations
                        Trial new_trial = t;
                        new_trial.buttons[new_trial.len_buttons] = m.buttons[j];
                        new_trial.len_buttons++;

                        // but only once each!
                        int hash_buttons[new_trial.len_buttons];
                        for (int k = 0; k < new_trial.len_buttons; ++k) {
                            hash_buttons[k] = new_trial.buttons[k];
                        }
                        qsort(hash_buttons, new_trial.len_buttons, sizeof(int), compare_int);

                        uint64_t trial_hash = spookyhash64(hash_buttons, new_trial.len_buttons * sizeof(int), hashseed);
                        bool seen = false;
                        for (int k = 0; k < len_seen_buttons; ++k) {
                            if (trial_hash == seen_buttons[k]) {
                                seen = true;
                                ++seen_count;
                                break;
                            }
                        }
                        if (!seen) {
                            if (len_seen_buttons == capacity_seen_buttons) {
                                capacity_seen_buttons *= 2;
                                seen_buttons = realloc(seen_buttons, capacity_seen_buttons * sizeof(uint64_t));
                            }
                            assert(len_seen_buttons < capacity_seen_buttons);
                            seen_buttons[len_seen_buttons++] = trial_hash;
                            insert_heap(&heap, new_trial);
                        }
                    }
                } 
            }
        }
        printf("Repeated trials avoided for this machine: %lu\n", seen_count);
        free(seen_buttons);
        free(heap.trials);
    }
    printf("Part 2: %d\n", part2_sum);
}

int compare_int(const void *a, const void *b) {
    const int *ia = a;
    const int *ib = b;
    return (*ia > *ib) - (*ia < *ib);
}