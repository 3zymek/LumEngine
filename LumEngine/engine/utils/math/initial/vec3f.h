#pragma once
#ifdef __cplusplus
extern "C" {
#endif

	#include <math.h>
	#include <cstdio>
	typedef struct 
	
	{
		float x;
		float y;
		float z;
	} vec3f;

	static inline vec3f vec3f_create(float _x, float _y, float _z) {
		return (vec3f)(_x, _y, _z);
	}

	static inline void vec3f_add(const vec3f* a, const vec3f* b, vec3f* out) {
		out->x = a->x + b->x;
		out->y = a->y + b->y;
		out->z = a->z + b->z;
	}

	static inline void vec3f_sub(const vec3f* a, const vec3f* b, vec3f* out) {
		out->x = a->x - b->x;
		out->y = a->y - b->y;
		out->z = a->z - b->z;
	}

	static inline void vec3f_mul(const vec3f* a, const vec3f* b, vec3f* out) {
		out->x = a->x * b->x;
		out->y = a->y * b->y;
		out->z = a->z * b->z;
	}

	static inline void vec3f_mul_scalar(const vec3f* a, float scalar, vec3f* out) {
		out->x = a->x * scalar;
		out->y = a->y * scalar;
		out->z = a->z * scalar;
	}

	static inline void vec3f_div(const vec3f* a, const vec3f* b, vec3f* out) {
		out->x = a->x / b->x;
		out->y = a->y / b->y;
		out->z = a->z / b->z;
	}

	static inline void vec3f_div_scalar(const vec3f* a, float scalar, vec3f* out) {
		out->x = a->x / scalar;
		out->y = a->y / scalar;
		out->z = a->z / scalar;
	}

	static inline void vec3f_neg(const vec3f* a, vec3f* out) {
		out->x = -a->x;
		out->y = -a->y;
		out->z = -a->z;
	}

	static inline float vec3f_dot(const vec3f* a, const vec3f* b) {
		return a->x * b->x + a->y * b->y + a->z * b->z;
	}

	static inline float vec3f_length(const vec3f* a) {
		return sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
	}

	static inline float vec3f_length_squared(const vec3f* a) {
		return a->x * a->x + a->y * a->y + a->z * a->z;
	}

	static inline void vec3f_normalize(const vec3f* a, float length, vec3f* out) {
		out->x = a->x / length;
		out->y = a->y / length;
		out->z = a->z / length;
	}

	static inline void vec3f_cross(const vec3f* a, const vec3f* b, vec3f* out) {
		out->x = a->y * b->z - a->z * b->y;
		out->y = a->z * b->x - a->x * b->z;
		out->z = a->x * b->y - a->y * b->x;
	}

	static inline bool vec3f_is_zero(const vec3f* a) {
		return a->x == 0 && a->y == 0 && a->z == 0;
	}

	static inline void vec3f_print(const vec3f* out) {
		printf("vec3f = (%f, %f, %f)\n", out->x, out->y, out->z);
	}

#ifdef __cplusplus
}
#endif