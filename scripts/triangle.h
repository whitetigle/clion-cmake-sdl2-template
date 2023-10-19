//
// Created by conta on 10/19/2023.
//

#ifndef PROJECT_NAME_TRIANGLE_H
#define PROJECT_NAME_TRIANGLE_H

#include "vector.h"

typedef struct {
    int a;
    int b;
    int c;
} face_t;

typedef struct {
    vec2_t points[3];
} triangle_t;

#endif //PROJECT_NAME_TRIANGLE_H
