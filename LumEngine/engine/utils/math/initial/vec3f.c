#include "vec3f.h"

void vec3f_add(const vec3f* a, const vec3f* b, vec3f* out) {
    out->x = a->x + b->x;
    out->y = a->y + b->y;
    out->z = a->z + b->z;
}

void vec3f_sub(const vec3f* a, const vec3f* b, vec3f* out) {
    out->x = a->x - b->x;
    out->y = a->y - b->y;
    out->z = a->z - b->z;
}

void vec3f_mul(const vec3f* a, const vec3f* b, vec3f* out) {
    out->x = a->x * b->x;
    out->y = a->y * b->y;
    out->z = a->z * b->z;
}

void vec3f_mul_scalar(const vec3f* a, float scalar, vec3f* out) {
    out->x = a->x * scalar;
    out->y = a->y * scalar;
    out->z = a->z * scalar;
}

void vec3f_div(const vec3f* a, const vec3f* b, vec3f* out) {
    out->x = a->x / b->x;
    out->y = a->y / b->y;
    out->z = a->z / b->z;
}

void vec3f_div_scalar(const vec3f* a, float scalar, vec3f* out) {
    out->x = a->x / scalar;
    out->y = a->y / scalar;
    out->z = a->z / scalar;
}

void vec3f_neg(const vec3f* a, vec3f* out) {
    out->x = -a->x;
    out->y = -a->y;
    out->z = -a->z;
}

void vec3f_normalize(const vec3f* a, float length, vec3f* out) {
    out->x = a->x / length;
    out->y = a->y / length;
    out->z = a->z / length;
}

void vec3f_cross(const vec3f* a, const vec3f* b, vec3f* out) {
    out->x = a->y * b->z - a->z * b->y;
    out->y = a->z * b->x - a->x * b->z;
    out->z = a->x * b->y - a->y * b->x;
}
