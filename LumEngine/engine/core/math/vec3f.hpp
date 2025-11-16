#pragma once
#include "initial/vec3f.h"
#include "core/details/e_define.hpp"

// Wrapper for Vector3D from C to C++
class Vec3f {
private:
	vec3f data;
public:

	Vec3f(float _x = 0.f, float _y = 0.f, float _z = 0.f) noexcept {
		data = vec3f_create(_x, _y, _z);
	}
	Vec3f(const vec3f& _v) : data(_v) {}

	force_inline float& x() noexcept { return data.x; }
	force_inline float x() const noexcept { return data.x; }
	force_inline float& y() noexcept { return data.y; }
	force_inline float y() const noexcept { return data.y; }
	force_inline float& z() noexcept { return data.z; }
	force_inline float z() const noexcept { return data.z; }

	force_inline Vec3f operator+(const Vec3f& oth) const {
		Vec3f v;
		vec3f_add(&data, &oth.data, &v.data);
		return v;
	}
	force_inline Vec3f& operator+=(const Vec3f& oth) {
		vec3f_add(&data, &oth.data, &data);
		return *this;
	}

	force_inline Vec3f operator-(const Vec3f& oth) const {
		Vec3f v;
		vec3f_sub(&data, &oth.data, &v.data);
		return v;
	}
	force_inline Vec3f& operator-=(const Vec3f& oth) {
		vec3f_sub(&data, &oth.data, &data);
		return *this;
	}
	Vec3f operator-() const {
		Vec3f v;
		vec3f_neg(&data, &v.data);
		return v;
	}


	force_inline Vec3f operator*(const Vec3f& oth) const {
		Vec3f v;
		vec3f_mul(&data, &oth.data, &v.data);
		return v;
	}
	force_inline Vec3f& operator*=(const Vec3f& oth) {
		vec3f_mul(&data, &oth.data, &data);
		return *this;
	}
	force_inline Vec3f operator*(float scalar) const {
		Vec3f v;
		vec3f_mul_scalar(&data, scalar, &v.data);
		return v;
	}
	force_inline Vec3f& operator*=(float scalar) {
		vec3f_mul_scalar(&data, scalar, &data);
		return *this;
	}

	force_inline Vec3f operator/(const Vec3f& oth) const {
		Vec3f v;
		vec3f_div(&data, &oth.data, &v.data);
		return v;
	}
	force_inline Vec3f& operator/=(const Vec3f& oth) {
		vec3f_div(&data, &oth.data, &data);
		return *this;
	}
	force_inline Vec3f operator/(float scalar) const {
		Vec3f v;
		vec3f_div_scalar(&data, scalar, &v.data);
		return v;
	}
	force_inline Vec3f& operator/=(float scalar) {
		vec3f_div_scalar(&data, scalar, &data);
		return *this;
	}

	force_inline float length() const {
		return vec3f_length(&data);
	}
	force_inline float length_squared() const {
		return vec3f_length_squared(&data);
	}
	force_inline float dot(const Vec3f& oth) const {
		return vec3f_dot(&data, &oth.data);
	}
	force_inline bool is_zero() const {
		return vec3f_is_zero(&data);
	}
	force_inline Vec3f cross(const Vec3f& oth) const {
		Vec3f v;
		vec3f_cross(&data, &oth.data, &v.data);
		return v;
	}
	force_inline Vec3f normalized() const {
		Vec3f v;
		vec3f_normalize(&data, length_squared(), &v.data);
		return v;
	}
	force_inline void print() const {
		vec3f_print(&data);
	}

};