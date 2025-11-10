#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>

    typedef struct vec3u {
        unsigned int x;
        unsigned int y;
        unsigned int z;
    } vec3u;

    static inline vec3u vec3u_create(unsigned int _x, unsigned int _y, unsigned int _z) {
        vec3u v = { _x, _y, _z };
        return v;
    }

    static inline bool vec3u_is_zero(const vec3u* a) {
        return a->x == 0 && a->y == 0 && a->z == 0;
    }

    static inline void vec3u_print(const vec3u* a) {
        printf("vec3u = (%u, %u, %u)\n", a->x, a->y, a->z);
    }

    void vec3u_add(const vec3u* a, const vec3u* b, vec3u* out);
    void vec3u_sub(const vec3u* a, const vec3u* b, vec3u* out);
    void vec3u_mul(const vec3u* a, const vec3u* b, vec3u* out);
    void vec3u_mul_scalar(const vec3u* a, unsigned int scalar, vec3u* out);
    void vec3u_div(const vec3u* a, const vec3u* b, vec3u* out);
    void vec3u_div_scalar(const vec3u* a, unsigned int scalar, vec3u* out);
    void vec3u_neg(const vec3u* a, vec3u* out);

#ifdef __cplusplus
}
#endif
