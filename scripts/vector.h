//
// Created by conta on 10/18/2023.
//

#ifndef PROJECT_NAME_VECTOR_H
#define PROJECT_NAME_VECTOR_H

typedef struct {
    float x;
    float y;
} vec2_t;

typedef struct {
    float x;
    float y;
    float z;
} vec3_t;

vec3_t vec3_rotate_x(vec3_t v, float angle);
vec3_t vec3_rotate_y(vec3_t v, float angle);
vec3_t vec3_rotate_z(vec3_t v, float angle);

#endif //PROJECT_NAME_VECTOR_H