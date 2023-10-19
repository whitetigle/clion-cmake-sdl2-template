//
// Created by conta on 10/18/2023.
//

#include "vector.h"

vec3_t vec3_rotate_z(vec3_t v, float angle) {
    vec3_t rotated_vector = {
            .x = v.x * cos(angle) - v.y * sin(angle),
            .y = v.y * cos(angle) + v.x * sin(angle),
            .z = v.z // lock z
    };
    return rotated_vector;
}

vec3_t vec3_rotate_y(vec3_t v, float angle) {
    vec3_t rotated_vector = {
            .x = v.x * cos(angle) - v.z * sin(angle),
            .y = v.y, // lock y
            .z = v.z * cos(angle) + v.x * sin(angle)
    };
    return rotated_vector;
}

vec3_t vec3_rotate_x(vec3_t v, float angle) {
    vec3_t rotated_vector = {
            .x = v.x,// lock x
            .y = v.y * cos(angle) - v.z * sin(angle),
            .z = v.z * cos(angle) + v.y * sin(angle)
    };
    return rotated_vector;
}