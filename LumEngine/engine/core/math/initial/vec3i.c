#include "vec3i.h"

void vec3i_add(const vec3i* a, const vec3i* b, vec3i* out) {
    out->x = a->x + b->x;
    out->y = a->y + b->y;
    out->z = a->z + b->z;
}

void vec3i_sub(const vec3i* a, const vec3i* b, vec3i* out) {
    out->x = a->x - b->x;
    out->y = a->y - b->y;
    out->z = a->z - b->z;
}

void vec3i_mul(const vec3i* a, const vec3i* b, vec3i* out) {
    out->x = a->x * b->x;
    out->y = a->y * b->y;
    out->z = a->z * b->z;
}

void vec3i_mul_scalar(const vec3i* a, int scalar, vec3i* out) {
    out->x = a->x * scalar;
    out->y = a->y * scalar;
    out->z = a->z * scalar;
}

void vec3i_div(const vec3i* a, const vec3i* b, vec3i* out) {
    out->x = a->x / b->x;
    out->y = a->y / b->y;
    out->z = a->z / b->z;
}

void vec3i_div_scalar(const vec3i* a, int scalar, vec3i* out) {
    out->x = a->x / scalar;
    out->y = a->y / scalar;
    out->z = a->z / scalar;
}

void vec3i_neg(const vec3i* a, vec3i* out) {
    out->x = -a->x;
    out->y = -a->y;
    out->z = -a->z;
}
