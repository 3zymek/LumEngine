#include "vec3u.h"

void vec3u_add(const vec3u* a, const vec3u* b, vec3u* out) {
    out->x = a->x + b->x;
    out->y = a->y + b->y;
    out->z = a->z + b->z;
}

void vec3u_sub(const vec3u* a, const vec3u* b, vec3u* out) {
    out->x = a->x - b->x;
    out->y = a->y - b->y;
    out->z = a->z - b->z;
}

void vec3u_mul(const vec3u* a, const vec3u* b, vec3u* out) {
    out->x = a->x * b->x;
    out->y = a->y * b->y;
    out->z = a->z * b->z;
}

void vec3u_mul_scalar(const vec3u* a, unsigned int scalar, vec3u* out) {
    out->x = a->x * scalar;
    out->y = a->y * scalar;
    out->z = a->z * scalar;
}

void vec3u_div(const vec3u* a, const vec3u* b, vec3u* out) {
    out->x = a->x / b->x;
    out->y = a->y / b->y;
    out->z = a->z / b->z;
}

void vec3u_div_scalar(const vec3u* a, unsigned int scalar, vec3u* out) {
    out->x = a->x / scalar;
    out->y = a->y / scalar;
    out->z = a->z / scalar;
}

void vec3u_neg(const vec3u* a, vec3u* out) {
    out->x = 0 - a->x;
    out->y = 0 - a->y;
    out->z = 0 - a->z;
}
