#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../common.h"


struct PrintingRoomPosition {
    int x;
    int y;
    bool has_paper;
    struct PrintingRoomPosition *north;
    struct PrintingRoomPosition *south;
    struct PrintingRoomPosition *east;
    struct PrintingRoomPosition *west;
};

struct PrintingRoom {
    size_t width;
    size_t height;
    size_t count;
    struct PrintingRoomPosition positions[];
};

struct PrintingRoom* parsePrintingRoom(char *buf);
bool qualifies_part1(struct PrintingRoomPosition *p);

int main(int argc, char **argv) {
    const char *input_path = (argc > 1) ? argv[1] : "day04/sample.txt";
    if (input_path == NULL) {
        printf("input path required\n");
        exit(EXIT_FAILURE);
    }
    long len;
    char *buf = read_file(input_path, &len);

    struct PrintingRoom *room = parsePrintingRoom(buf);

    unsigned int part1_count = 0;
    for (unsigned int i = 0; i < room->count; i++) {
        struct PrintingRoomPosition *p = &(room->positions[i]);
        if (qualifies_part1(p)) {
            printf("found one %d, %d\n", p->x, p->y);
            ++part1_count;
        }
    }

    printf("Part 1: %d\n", part1_count);

    free(room);
    free(buf);
}

bool qualifies_part1(struct PrintingRoomPosition *p) {
    const unsigned int max = 3;
    unsigned int neighbor_count = 0;
    if (!p->has_paper) return false;

    if (p->north) {
        if (p->north->has_paper) ++neighbor_count;
        if (p->north->west && p->north->west->has_paper) ++neighbor_count;
        if (p->north->east && p->north->east->has_paper) ++neighbor_count;
    }
    if (p->west && p->west->has_paper) ++neighbor_count;
    if (p->east && p->east->has_paper) ++neighbor_count;
    if (p->south) {
        if (p->south->has_paper) ++neighbor_count;
        if (p->south->west && p->south->west->has_paper) ++neighbor_count;
        if (p->south->east && p->south->east->has_paper) ++neighbor_count;
    }
    return neighbor_count <= max;
}

struct PrintingRoom* parsePrintingRoom(char *buf) {
    size_t position_count = 0;
    size_t width = 0;
    size_t height = 0;
    for (char *c = buf; *c != '\0'; ++c) {
        switch (*c) {
            case '.':
            case '@':
                position_count++;
                if (height == 0) {
                    width++;
                }
                break;
            case '\n':
                height++;
        }
    }
    // last line isn't newline terminated
    height++;

    struct PrintingRoom *room = malloc(sizeof(struct PrintingRoom) + position_count * sizeof(struct PrintingRoomPosition));
    room->height = height;
    room->width = width;
    room->count = position_count;
    struct PrintingRoomPosition *positions = room->positions;

    int x = 0;
    int y = 0;
    size_t pos = 0;
    for (char *c = buf; *c != '\0'; ++c) {
        switch (*c) {
            case '.':
            case '@':
                struct PrintingRoomPosition *p = &positions[pos];
                p->x = x++;
                p->y = y;
                p->has_paper = (*c == '@');
                if (pos < width) {
                    p->north = NULL;
                } else {
                    p->north = &positions[pos - width]; // y - 1
                }
                if (pos >= ((height - 1) * width)) {
                    p->south = NULL;
                } else {
                    p->south = &positions[pos + width]; // y + 1
                }
                if (pos % width == width - 1) {
                    p->east = NULL;
                } else {
                    p->east = &positions[pos + 1]; // x + 1
                }
                if (pos % width == 0) {
                    p->west = NULL;  
                } else {
                    p->west = &positions[pos - 1]; // x - 1
                }
                pos++;
                break;
            case '\n':
                x = 0;
                y++;
                break;
        }
    }

    return room;
}