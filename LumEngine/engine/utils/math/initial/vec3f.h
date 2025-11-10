#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <stdio.h>
#include <stdbool.h>

    typedef struct vec3f {
        float x;
        float y;
        float z;
    } vec3f;

    static inline vec3f vec3f_create(float _x, float _y, float _z) {
        vec3f v = { _x, _y, _z };
        return v;
    }

    static inline float vec3f_dot(const vec3f* a, const vec3f* b) {
        return a->x * b->x + a->y * b->y + a->z * b->z;
    }

    static inline float vec3f_length(const vec3f* a) {
        return sqrtf(a->x * a->x + a->y * a->y + a->z * a->z);
    }

    static inline float vec3f_length_squared(const vec3f* a) {
        return a->x * a->x + a->y * a->y + a->z * a->z;
    }

    static inline bool vec3f_is_zero(const vec3f* a) {
        return a->x == 0 && a->y == 0 && a->z == 0;
    }

    static inline void vec3f_print(const vec3f* a) {
        printf("vec3f = (%f, %f, %f)\n", a->x, a->y, a->z);
    }

    void vec3f_add(const vec3f* a, const vec3f* b, vec3f* out);
    void vec3f_sub(const vec3f* a, const vec3f* b, vec3f* out);
    void vec3f_mul(const vec3f* a, const vec3f* b, vec3f* out);
    void vec3f_mul_scalar(const vec3f* a, float scalar, vec3f* out);
    void vec3f_div(const vec3f* a, const vec3f* b, vec3f* out);
    void vec3f_div_scalar(const vec3f* a, float scalar, vec3f* out);
    void vec3f_neg(const vec3f* a, vec3f* out);
    void vec3f_normalize(const vec3f* a, float length, vec3f* out);
    void vec3f_cross(const vec3f* a, const vec3f* b, vec3f* out);

#ifdef __cplusplus
}
#endif
