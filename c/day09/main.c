#include <stdlib.h>
#include <math.h>
#include "../common.h"
#include "main.h"

Point tiles[1024];
size_t len_tiles = 0;

void parse_tiles(char *input) {
    char **lines = split(input, '\n', &len_tiles);
    for (size_t i = 0; i < len_tiles; ++i) {
        sscanf(lines[i], "%lf,%lf", &tiles[i].x, &tiles[i].y);
    }
}

int compare_area_desc(const void *a, const void *b) {
    Rectangle *r1 = (Rectangle *) a;
    Rectangle *r2 = (Rectangle *) b;
    if (r2->area > r1->area) return 1;
    else if (r2->area < r1->area) return -1;
    else return 0;
}

int main(int argc, char **argv) {
    const char *input_path = (argc > 1) ? argv[1] : "day09/sample.txt";
    if (input_path == NULL) {
        printf("input path required\n");
        exit(EXIT_FAILURE);
    }
    long len;
    char *buf = read_file(input_path, &len);
    parse_tiles(buf);
    Rectangle *rectangles = malloc(300000 * sizeof(Rectangle));
    size_t len_rectangles = 0;

    double largest_area = 0;
    for (size_t i = 0; i < len_tiles; ++i) {
        for (size_t j = 0; j < len_tiles; ++j) {
            double area = (fabs(tiles[j].x - tiles[i].x) + 1) * (fabs(tiles[j].y - tiles[i].y) + 1);
            if (area > largest_area) largest_area = area;
            Rectangle *r = &rectangles[len_rectangles++];
            r->p1 = tiles[i];
            r->p2 = tiles[j];
            r->area = area;
        }
    }
    printf("Part 1: %.0f\n", largest_area);

    // Part 2 - region membership
    // https://www.eecs.umich.edu/courses/eecs380/HANDOUTS/PROJ2/InsidePoly.html

    // idea - first define the green region (done for me, tiles array is already that)
    // then - sort the rectangles by size
    qsort(rectangles, len_rectangles, sizeof(Rectangle), &compare_area_desc);
    
    // then - iterate the rectangles 
    for (size_t i = 0; i < len_rectangles; ++i) {
        Rectangle r = rectangles[i];
        if (fabs(r.p1.y - r.p2.y) > 50000) {
            // printf("skipping too tall\n");
            continue;
        }
        bool valid = true;
        // check for any vertices inside (exclusive), 
        double leftX = (r.p1.x < r.p2.x) ? r.p1.x : r.p2.x;
        double rightX = (r.p2.x > r.p1.x) ? r.p2.x : r.p1.x;
        double topY = (r.p1.y < r.p2.y) ? r.p1.y : r.p2.y;
        double bottomY = (r.p2.y > r.p1.y) ? r.p2.y : r.p1.y;
        for (size_t j = 0; j < len_tiles; ++j) {
            if (tiles[j].x > leftX && tiles[j].x < rightX && tiles[j].y > topY && tiles[j].y < bottomY) {
                // tile inside the rectangle! one side or the other must not be green or red, disqualify this rectangle
                valid = false;
                break;
            }
        }
        if (!valid) continue;
        // if not, check if the diagonal is inside the region
        if (rightX != leftX) {
            double slope = (bottomY - topY) / (rightX - leftX);
            // y = mx + b ; b = y - mx
            double intercept = topY - slope * leftX;
            // printf("slope %f, intercept %f\n", slope, intercept);
            for (int j = leftX; j < rightX; ++j) {
                double testX = (double) j;
                double testY = slope * testX + intercept;

                if (round(testX) == leftX || round(testX) == rightX || round(testY) == topY || round(testY) == bottomY) {
                    // border case, ignore
                    continue;
                }
                Point testP = {.x = testX, .y = testY};
                if (!point_in_polygon(tiles, len_tiles, testP)) {
                    // disqualified
                    // printf("disqualified %.0f,%.0f - %.0f,%.0f: %.1f,%.1f\n", r.p1.x, r.p1.y, r.p2.x, r.p2.y, testX, testY);
                    valid = false;
                    break;
                }
            }
        } else {
            printf("skipping with left %f right %f\n", leftX, rightX);
        }
        if (!valid) continue;

        // winner!
        printf("Found good rectangle! %.0f,%.0f - %.0f,%.0f\n", r.p1.x, r.p1.y, r.p2.x, r.p2.y);
        printf("Part 2: %.0f\n", r.area);
        break;
    }


    free(rectangles);
    free(buf);
}

// adapted from Randolph Franklin's implementation - so tight!
// https://wrfranklin.org/Research/Short_Notes/pnpoly.html
bool point_in_polygon(Point* polygon, int len_polygon, Point p) {
    int i, j = 0;
    bool inside = false;
    for (i = 0, j = len_polygon - 1; i < len_polygon; j = i++) {
        if ( 
            ((polygon[i].y > p.y) != (polygon[j].y > p.y)) &&
            (p.x < (polygon[j].x - polygon[i].x) * (p.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x )
        ) {
            inside = !inside;
        }
    }
    return inside;
}