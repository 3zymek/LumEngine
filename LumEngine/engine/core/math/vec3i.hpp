#pragma once
#include "initial/vec3i.h"
#include "e_define.hpp"

// Wrapper for Vector3D from C to C++
class Vec3i {
private:
	vec3i data;
public:

	Vec3i(int _x = 0.f, int _y = 0.f, int _z = 0.f) noexcept {
		data = vec3i_create(_x, _y, _z);
	}
	Vec3i(const vec3i& _v) : data(_v) {}

	force_inline int& x() noexcept { return data.x; }
	force_inline int x() const noexcept { return data.x; }
	force_inline int& y() noexcept { return data.y; }
	force_inline int y() const noexcept { return data.y; }
	force_inline int& z() noexcept { return data.z; }
	force_inline int z() const noexcept { return data.z; }

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

	force_inline 
	force_inline bool is_zero() const {
		return vec3i_is_zero(&data);
	}
	force_inline void print() const {
		vec3i_print(&data);
	}

};