#include <stdbool.h>

typedef struct {
    double x, y;
} Point;

typedef struct {
    Point p1, p2;
    double area;
} Rectangle;

bool point_in_polygon(Point* polygon, int len_polygons, Point p);
