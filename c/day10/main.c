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
uint64_t spookyseed = 424242424242;
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

    for (size_t i = 0; i < len_machines; ++i) {
        Machine m = machines[i];
        part2_sum += find_button_presses(&m);
    }

    printf("Part 2: %d\n", part2_sum);
}

int compare_int(const void *a, const void *b) {
    const int *ia = a;
    const int *ib = b;
    return (*ia > *ib) - (*ia < *ib);
}

bool joltage_match(Machine *m, int *joltage) {
    for (int i = 0; i < m->len_joltage; ++i) {
        if (joltage[i] != m->joltage[i]) {
            return false;
        }
    }
    return true;
}

void apply_buttons(Machine *m, Trial *t, int *joltage) {
    for (int b = 0; b < m->len_buttons; ++b) {
        for (int c = 0; c < m->len_joltage; ++c) {
            if (button_influences_counter(m, b, c)) {
                joltage[c] += t->counts[b];
            }
        }
    }
}


int find_button_presses(Machine *machine) {
    int presses_required = 0;
    TrialHeap *heap = malloc(sizeof(TrialHeap));
    heap->size = 0;
    heap->capacity = 256;
    heap->trials = malloc(heap->capacity * sizeof(Trial));

    Trial trial = {
        .len_buttons = 0,
        .machine = *machine,
    };
    for (int i = 0; i < machine->len_buttons; ++i) {
        trial.counts[i] = 0;
    }
    insert_heap(heap, trial);
    uint64_t *seen = malloc(2048 * sizeof(uint64_t));
    int capacity_seen = 2048;
    int len_seen = 0;
    int cache_hits = 0;

    while (presses_required == 0) {
        trial = extract_heap_min(heap); // program will crash here if the heap is empty which is fine
        bool seen_this = false;
        uint64_t this_hash = spookyhash64(trial.counts, sizeof(trial.counts), spookyseed);
        for (int i = 0; i < len_seen; ++i) {
            if (seen[i] == this_hash) {
                seen_this = true;
                cache_hits++;
                break;
            }
        }
        if (seen_this) continue;
        if (len_seen == capacity_seen) {
            capacity_seen *= 2;
            seen = realloc(seen, capacity_seen * sizeof(uint64_t));
        }
        seen[len_seen++] = this_hash;
        for (int i = 0; i < machine->len_buttons; ++i) {
            bool loud = false;
            int cum_joltage[machine->len_joltage];
            memset(cum_joltage, 0, machine->len_joltage * sizeof(int));
            apply_buttons(machine, &trial, cum_joltage);
            if (loud) {
                printf("checking trial for ");
                for (int j = 0; j < machine->len_buttons; ++j) {
                    printf("%d ", trial.counts[j]);
                }
                printf("button %d\n", i);
                printf("joltage ");
                for (int j = 0; j < machine->len_joltage; ++j) {
                    printf("%d ", cum_joltage[j]);
                }                
            }

            if (presses_required == 0) {
                if (joltage_match(machine, cum_joltage)) {
                    // we're done
                    printf("joltage ");
                    for (int j = 0; j < machine->len_joltage; ++j) {
                        printf("%d ", cum_joltage[j]);
                    }
                    printf("\n");
                    presses_required = trial.len_buttons;
                    printf("Found buttons %d\n", trial.len_buttons);
                    break;
                } else {
                    // enumerate all possibilities using this button
                    for (int c = 0; c < machine->len_joltage; ++c) {
                        for (int j = 0; j < machine->joltage[c] - cum_joltage[c] + 1; ++j) {
                            Trial next_trial;
                            memcpy(&next_trial, &trial, sizeof(next_trial));
                            // bump all counters influenced by this button
                            bool overflow = false;
                            int next_joltage[machine->len_joltage];
                            memset(next_joltage, 0, machine->len_joltage * sizeof(int));
                            for (int k = 0; k < machine->len_joltage; ++k) {
                                if (button_influences_counter(machine, i, k)) {
                                    next_joltage[k] += j; // j is how many presses, k is the counter, i is the button
                                    overflow = overflow || (next_joltage[k] > machine->joltage[k]);
                                }
                            }
                            if (!overflow) {
                                next_trial.counts[i] += j;
                                next_trial.len_buttons += j;
                                insert_heap(heap, next_trial);
                            }
                        }
                    }
                }
            }
        }
    }
    free(seen);
    free(heap->trials);
    free(heap);
    return presses_required;
}

bool button_influences_counter(Machine *m, int button_index, int counter) {
    if (counter >= m->len_joltage) return false;
    // printf("button %d is %d\n", button_index, m->buttons[button_index]);
    // printf("button %d shifted %d is %d\n", button_index, (m->len_joltage - counter - 1), (m->buttons[button_index] >> (m->len_joltage - counter - 1)));
    return ((m->buttons[button_index] >> (m->len_joltage - counter - 1)) % 2);
}