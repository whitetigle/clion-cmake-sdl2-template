//
// Created by conta on 10/19/2023.
//

#ifndef PROJECT_NAME_TRIANGLE_H
#define PROJECT_NAME_TRIANGLE_H

#include <stdint.h>
#include "vector.h"

typedef struct {
    int a;
    int b;
    int c;
} face_t;

typedef struct {
    vec2_t points[3];
} triangle_t;

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
#endif //PROJECT_NAME_TRIANGLE_H
