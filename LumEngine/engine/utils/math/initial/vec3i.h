#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>

    typedef struct vec3i {
        int x;
        int y;
        int z;
    } vec3i;

    static inline vec3i vec3i_create(int _x, int _y, int _z) {
        vec3i v = { _x, _y, _z };
        return v;
    }

    static inline bool vec3i_is_zero(const vec3i* a) {
        return a->x == 0 && a->y == 0 && a->z == 0;
    }

    static inline void vec3i_print(const vec3i* a) {
        printf("vec3i = (%d, %d, %d)\n", a->x, a->y, a->z);
    }

    void vec3i_add(const vec3i* a, const vec3i* b, vec3i* out);
    void vec3i_sub(const vec3i* a, const vec3i* b, vec3i* out);
    void vec3i_mul(const vec3i* a, const vec3i* b, vec3i* out);
    void vec3i_mul_scalar(const vec3i* a, int scalar, vec3i* out);
    void vec3i_div(const vec3i* a, const vec3i* b, vec3i* out);
    void vec3i_div_scalar(const vec3i* a, int scalar, vec3i* out);
    void vec3i_neg(const vec3i* a, vec3i* out);

#ifdef __cplusplus
}
#endif
