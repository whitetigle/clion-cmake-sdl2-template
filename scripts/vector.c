//
// Created by conta on 10/18/2023.
//

/*
 * given the coordinates (x,y)
 *
 * we have a triangle whose
 * - adjacent side's length is equal to y
 * - opposite side's length is equal to x
 * - hypothenuse side's length is r. It is the radius in a circle. It does not change over time in the context of that circle
 *
 * x = r cos α
 * y = r sin α
 *
 * if we wish to rotate a vector we need to calculate the new x and y (x',y') of a new triangle whose angle is β.
 * The total angle will be α + β
 * The formula will thus be :
 *
 * x' = r cos (α + β)
 * y' = r sin (α + β)
 *
 * We know that r does not change since it's the length of the radius of the circle we're working in
 *
 * so we can calculate x' for instance using the *Angle Addition Formula* for cosine
 * x' = r cos (α + β)
 * <=> x' = r (cos α cos β - sin α sin β)  (= Angle Addition Formula)
 * <=> x' = r cos α cos β - r sin α sin β
 * <=> x' = x cos β - y sin β (x = r cosα and y = r sin α)
 *
 * We can derivate this function for y' using the same logic and we'll get
 * y' = y cos β - x sin β
 *
 * then for each vector we just need to calculate 2 values and lock the third one
 */

#include <math.h>
#include "vector.h"

float vec2_length(vec2_t v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

vec2_t vec2_add(vec2_t a, vec2_t b) {
    vec2_t result  = {
            a.x + b.x,
            a.y+b.y};
    return result;
}

vec2_t vec2_sub(vec2_t a, vec2_t b) {
    vec2_t result  = {
            a.x - b.x,
            a.y - b.y};
    return result;
}

vec2_t vec2_mul(vec2_t a, float factor) {
    vec2_t result  = {
            a.x * factor,
            a.y * factor};
    return result;
}

vec2_t vec2_div(vec2_t a, float factor) {
    vec2_t result  = {
            a.x / factor,
            a.y / factor};
    return result;
}

float vec3_length(vec3_t v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3_t vec3_add(vec3_t a, vec3_t b) {
    vec3_t result  = {
            a.x + b.x,
            a.y + b.y,
            a.z + b.z};
    return result;
}

vec3_t vec3_sub(vec3_t a, vec3_t b) {
    vec3_t result  = {
            a.x - b.x,
            a.y - b.y,
            a.z -b .z
    };
    return result;
}

vec3_t vec3_mul(vec3_t a, float factor) {
    vec3_t result  = {
            a.x * factor,
            a.y * factor,
            a.z * factor
    };
    return result;
}

vec3_t vec3_div(vec3_t a, float factor) {
    vec3_t result  = {
            a.x / factor,
            a.y / factor,
            a.z / factor
    };
    return result;
}

vec3_t vec3_cross(vec3_t a, vec3_t b) {
    vec3_t result = {
            a.y * b.y - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
    };
    return result;
}



vec3_t vec3_rotate_z(vec3_t v, float angle) {
    vec3_t rotated_vector = {
            .x = v.x * cosf(angle) - v.y * sinf(angle),
            .y = v.y * cosf(angle) + v.x * sinf(angle),
            .z = v.z // lock z
    };
    return rotated_vector;
}

vec3_t vec3_rotate_y(vec3_t v, float angle) {
    vec3_t rotated_vector = {
            .x = v.x * cosf(angle) - v.z * sinf(angle),
            .y = v.y, // lock y
            .z = v.z * cosf(angle) + v.x * sinf(angle)
    };
    return rotated_vector;
}

vec3_t vec3_rotate_x(vec3_t v, float angle) {
    vec3_t rotated_vector = {
            .x = v.x,// lock x
            .y = v.y * cosf(angle) - v.z * sinf(angle),
            .z = v.z * cosf(angle) + v.y * sinf(angle)
    };
    return rotated_vector;
}