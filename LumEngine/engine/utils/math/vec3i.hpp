#pragma once
#include "initial/vec3i.h"
#include "e_define.hpp"

// Wrapper for Vector3D from C to C++
class Vec3i {
private:
	vec3i data;
public:

	Vec3i(float _x = 0.f, float _y = 0.f, float _z = 0.f) noexcept {
		data = vec3i_create(_x, _y, _z);
	}
	Vec3i(const vec3i& _v) : data(_v) {}

	force_inline float& x() noexcept { return data.x; }
	force_inline float x() const noexcept { return data.x; }
	force_inline float& y() noexcept { return data.y; }
	force_inline float y() const noexcept { return data.y; }
	force_inline float& z() noexcept { return data.z; }
	force_inline float z() const noexcept { return data.z; }

	force_inline Vec3i operator+(const Vec3i& oth) const {
		Vec3i v;
		vec3i_add(&data, &oth.data, &v.data);
		return v;
	}
	force_inline Vec3i& operator+=(const Vec3i& oth) {
		vec3i_add(&data, &oth.data, &data);
		return *this;
	}

	force_inline Vec3i operator-(const Vec3i& oth) const {
		Vec3i v;
		vec3i_sub(&data, &oth.data, &v.data);
		return v;
	}
	force_inline Vec3i& operator-=(const Vec3i& oth) {
		vec3i_sub(&data, &oth.data, &data);
		return *this;
	}
	Vec3i operator-() const {
		Vec3i v;
		vec3i_neg(&data, &v.data);
		return v;
	}


	force_inline Vec3i operator*(const Vec3i& oth) const {
		Vec3i v;
		vec3i_mul(&data, &oth.data, &v.data);
		return v;
	}
	force_inline Vec3i& operator*=(const Vec3i& oth) {
		vec3i_mul(&data, &oth.data, &data);
		return *this;
	}
	force_inline Vec3i operator*(float scalar) const {
		Vec3i v;
		vec3i_mul_scalar(&data, scalar, &v.data);
		return v;
	}
	force_inline Vec3i& operator*=(float scalar) {
		vec3i_mul_scalar(&data, scalar, &data);
		return *this;
	}

	force_inline Vec3i operator/(const Vec3i& oth) const {
		Vec3i v;
		vec3i_div(&data, &oth.data, &v.data);
		return v;
	}
	force_inline Vec3i& operator/=(const Vec3i& oth) {
		vec3i_div(&data, &oth.data, &data);
		return *this;
	}
	force_inline Vec3i operator/(float scalar) const {
		Vec3i v;
		vec3i_div_scalar(&data, scalar, &v.data);
		return v;
	}
	force_inline Vec3i& operator/=(float scalar) {
		vec3i_div_scalar(&data, scalar, &data);
		return *this;
	}

	force_inline float length() const {
		return vec3i_length(&data);
	}
	force_inline float length_squared() const {
		return vec3i_length_squared(&data);
	}
	force_inline float dot(const Vec3i& oth) const {
		return vec3i_dot(&data, &oth.data);
	}
	force_inline bool is_zero() const {
		return vec3i_is_zero(&data);
	}
	force_inline Vec3i cross(const Vec3i& oth) const {
		Vec3i v;
		vec3i_cross(&data, &oth.data, &v.data);
		return v;
	}
	force_inline Vec3i normalized() const {
		Vec3i v;
		vec3i_normalize(&data, length_squared(), &v.data);
		return v;
	}
	force_inline void print() const {
		vec3i_print(&data);
	}

};