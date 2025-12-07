#include <stdlib.h>
#include <strings.h>
#include "../common.h"

struct Problem {
    char operator;
    unsigned int operands[4];
    size_t operand_count;
};

struct Problem *parse_input(char *input, size_t *out_problem_count);

int main(int argc, char **argv) {
    const char *input_path = (argc > 1) ? argv[1] : "day06/sample.txt";
    if (input_path == NULL) {
        printf("input path required\n");
        exit(EXIT_FAILURE);
    }
    long len;
    char *buf = read_file(input_path, &len);

    size_t problem_count = 0;
    struct Problem *problems = parse_input(buf, &problem_count);

    unsigned long part1_total = 0;
    for (size_t i = 0; i < problem_count; i++) {
        struct Problem p = problems[i];
        unsigned long problem_result = p.operator == '+' ? 0 : 1;
        for (size_t j = 0; j < p.operand_count; j++) {
            if (p.operator == '+') {
                problem_result += p.operands[j];
            } else {
                problem_result *= p.operands[j];
            }
        }
        printf("%c %d %d %d = %lu\n", p.operator, p.operands[0], p.operands[1], p.operands[2], problem_result);
        part1_total += problem_result;
    }

    printf("Part 1: %lu\n", part1_total);

    free(problems);
    free(buf);
}

struct Problem *parse_input(char *input, size_t *out_problem_count) {
    size_t line_count = 0;
    char **lines = split(input, '\n', &line_count);

    char *operator_line = lines[line_count - 1];

    *out_problem_count = 0;
    size_t capacity = 8;
    struct Problem *problems = malloc(capacity * sizeof(struct Problem));
    
    for (size_t pos = 0; pos < strlen(operator_line); ++pos) {
        switch (operator_line[pos]) {
            case '*':
            case '+':
                if (out_problem_count == capacity) {
                    capacity *= 2;
                    problems = realloc(problems, capacity * sizeof(struct Problem));
                }
                struct Problem *p = &(problems[(*out_problem_count)++]);
                p->operator = operator_line[pos];
                p->operand_count = line_count - 1;
                for (size_t i = 0; i < line_count - 1; i++) {
                    sscanf(lines[i] + pos, "%d ", &(p->operands[i]));
                }
                break;
            case ' ':
                break;
            default:
                printf("unrecognized operator character %c\n", operator_line[pos]);
                exit(EXIT_FAILURE);
        }
    }  
    return problems;
}