#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <cstdio>
	typedef struct vec3i {
		float x;
		float y;
		float z;
	} vec3i;

	static inline vec3i vec3i_create(float _x, float _y, float _z) {
		return (vec3i)(_x, _y, _z);
	}

	static inline void vec3i_add(const vec3i* a, const vec3i* b, vec3i* out) {
		out->x = a->x + b->x;
		out->y = a->y + b->y;
		out->z = a->z + b->z;
	}

	static inline void vec3i_sub(const vec3i* a, const vec3i* b, vec3i* out) {
		out->x = a->x - b->x;
		out->y = a->y - b->y;
		out->z = a->z - b->z;
	}

	static inline void vec3i_mul(const vec3i* a, const vec3i* b, vec3i* out) {
		out->x = a->x * b->x;
		out->y = a->y * b->y;
		out->z = a->z * b->z;
	}

	static inline void vec3i_mul_scalar(const vec3i* a, float scalar, vec3i* out) {
		out->x = a->x * scalar;
		out->y = a->y * scalar;
		out->z = a->z * scalar;
	}

	static inline void vec3i_div(const vec3i* a, const vec3i* b, vec3i* out) {
		out->x = a->x / b->x;
		out->y = a->y / b->y;
		out->z = a->z / b->z;
	}

	static inline void vec3i_div_scalar(const vec3i* a, float scalar, vec3i* out) {
		out->x = a->x / scalar;
		out->y = a->y / scalar;
		out->z = a->z / scalar;
	}

	static inline void vec3i_neg(const vec3i* a, vec3i* out) {
		out->x = -a->x;
		out->y = -a->y;
		out->z = -a->z;
	}

	static inline float vec3i_dot(const vec3i* a, const vec3i* b) {
		return a->x * b->x + a->y * b->y + a->z * b->z;
	}

	static inline float vec3i_length(const vec3i* a) {
		return sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
	}

	static inline float vec3i_length_squared(const vec3i* a) {
		return a->x * a->x + a->y * a->y + a->z * a->z;
	}

	static inline void vec3i_normalize(const vec3i* a, float length, vec3i* out) {
		out->x = a->x / length;
		out->y = a->y / length;
		out->z = a->z / length;
	}

	static inline void vec3i_cross(const vec3i* a, const vec3i* b, vec3i* out) {
		out->x = a->y * b->z - a->z * b->y;
		out->y = a->z * b->x - a->x * b->z;
		out->z = a->x * b->y - a->y * b->x;
	}

	static inline bool vec3i_is_zero(const vec3i* a) {
		return a->x == 0 && a->y == 0 && a->z == 0;
	}

	static inline void vec3i_print(const vec3i* out) {
		printf("vec3i = (%f, %f, %f)\n", out->x, out->y, out->z);
	}

#ifdef __cplusplus
}
#endif