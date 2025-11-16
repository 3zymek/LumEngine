#pragma once
#include "initial/vec3u.h"
#include "e_define.hpp"

// Wrapper for Vector3D from C to C++
class Vec3u {
private:
	vec3u data;
public:

	Vec3u(float _x = 0.f, float _y = 0.f, float _z = 0.f) noexcept {
		data = vec3u_create(_x, _y, _z);
	}
	Vec3u(const vec3u& _v) : data(_v) {}

	force_inline float& x() noexcept { return data.x; }
	force_inline float x() const noexcept { return data.x; }
	force_inline float& y() noexcept { return data.y; }
	force_inline float y() const noexcept { return data.y; }
	force_inline float& z() noexcept { return data.z; }
	force_inline float z() const noexcept { return data.z; }

	force_inline Vec3u operator+(const Vec3u& oth) const {
		Vec3u v;
		vec3u_add(&data, &oth.data, &v.data);
		return v;
	}
	force_inline Vec3u& operator+=(const Vec3u& oth) {
		vec3u_add(&data, &oth.data, &data);
		return *this;
	}

	force_inline Vec3u operator-(const Vec3u& oth) const {
		Vec3u v;
		vec3u_sub(&data, &oth.data, &v.data);
		return v;
	}
	force_inline Vec3u& operator-=(const Vec3u& oth) {
		vec3u_sub(&data, &oth.data, &data);
		return *this;
	}
	Vec3u operator-() const {
		Vec3u v;
		vec3u_neg(&data, &v.data);
		return v;
	}


	force_inline Vec3u operator*(const Vec3u& oth) const {
		Vec3u v;
		vec3u_mul(&data, &oth.data, &v.data);
		return v;
	}
	force_inline Vec3u& operator*=(const Vec3u& oth) {
		vec3u_mul(&data, &oth.data, &data);
		return *this;
	}
	force_inline Vec3u operator*(float scalar) const {
		Vec3u v;
		vec3u_mul_scalar(&data, scalar, &v.data);
		return v;
	}
	force_inline Vec3u& operator*=(float scalar) {
		vec3u_mul_scalar(&data, scalar, &data);
		return *this;
	}

	force_inline Vec3u operator/(const Vec3u& oth) const {
		Vec3u v;
		vec3u_div(&data, &oth.data, &v.data);
		return v;
	}
	force_inline Vec3u& operator/=(const Vec3u& oth) {
		vec3u_div(&data, &oth.data, &data);
		return *this;
	}
	force_inline Vec3u operator/(float scalar) const {
		Vec3u v;
		vec3u_div_scalar(&data, scalar, &v.data);
		return v;
	}
	force_inline Vec3u& operator/=(float scalar) {
		vec3u_div_scalar(&data, scalar, &data);
		return *this;
	}

	force_inline float length() const {
		return vec3u_length(&data);
	}
	force_inline float length_squared() const {
		return vec3u_length_squared(&data);
	}
	force_inline float dot(const Vec3u& oth) const {
		return vec3u_dot(&data, &oth.data);
	}
	force_inline bool is_zero() const {
		return vec3u_is_zero(&data);
	}
	force_inline Vec3u cross(const Vec3u& oth) const {
		Vec3u v;
		vec3u_cross(&data, &oth.data, &v.data);
		return v;
	}
	force_inline Vec3u normalized() const {
		Vec3u v;
		vec3u_normalize(&data, length_squared(), &v.data);
		return v;
	}
	force_inline void print() const {
		vec3u_print(&data);
	}

};