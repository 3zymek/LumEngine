#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <cstdio>
	typedef struct vec3u {
		float x;
		float y;
		float z;
	} vec3u;

	static inline vec3u vec3u_create(float _x, float _y, float _z) {
		return (vec3u)(_x, _y, _z);
	}

	static inline void vec3u_add(const vec3u* a, const vec3u* b, vec3u* out) {
		out->x = a->x + b->x;
		out->y = a->y + b->y;
		out->z = a->z + b->z;
	}

	static inline void vec3u_sub(const vec3u* a, const vec3u* b, vec3u* out) {
		out->x = a->x - b->x;
		out->y = a->y - b->y;
		out->z = a->z - b->z;
	}

	static inline void vec3u_mul(const vec3u* a, const vec3u* b, vec3u* out) {
		out->x = a->x * b->x;
		out->y = a->y * b->y;
		out->z = a->z * b->z;
	}

	static inline void vec3u_mul_scalar(const vec3u* a, float scalar, vec3u* out) {
		out->x = a->x * scalar;
		out->y = a->y * scalar;
		out->z = a->z * scalar;
	}

	static inline void vec3u_div(const vec3u* a, const vec3u* b, vec3u* out) {
		out->x = a->x / b->x;
		out->y = a->y / b->y;
		out->z = a->z / b->z;
	}

	static inline void vec3u_div_scalar(const vec3u* a, float scalar, vec3u* out) {
		out->x = a->x / scalar;
		out->y = a->y / scalar;
		out->z = a->z / scalar;
	}

	static inline void vec3u_neg(const vec3u* a, vec3u* out) {
		out->x = -a->x;
		out->y = -a->y;
		out->z = -a->z;
	}

	static inline float vec3u_dot(const vec3u* a, const vec3u* b) {
		return a->x * b->x + a->y * b->y + a->z * b->z;
	}

	static inline float vec3u_length(const vec3u* a) {
		return sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
	}

	static inline float vec3u_length_squared(const vec3u* a) {
		return a->x * a->x + a->y * a->y + a->z * a->z;
	}

	static inline void vec3u_normalize(const vec3u* a, float length, vec3u* out) {
		out->x = a->x / length;
		out->y = a->y / length;
		out->z = a->z / length;
	}

	static inline void vec3u_cross(const vec3u* a, const vec3u* b, vec3u* out) {
		out->x = a->y * b->z - a->z * b->y;
		out->y = a->z * b->x - a->x * b->z;
		out->z = a->x * b->y - a->y * b->x;
	}

	static inline bool vec3u_is_zero(const vec3u* a) {
		return a->x == 0 && a->y == 0 && a->z == 0;
	}

	static inline void vec3u_print(const vec3u* out) {
		printf("vec3u = (%f, %f, %f)\n", out->x, out->y, out->z);
	}

#ifdef __cplusplus
}
#endif