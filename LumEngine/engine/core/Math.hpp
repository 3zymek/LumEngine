#pragma once
#include <cmath>
#include "e_define.hpp"

namespace math {

	constexpr float PI = 3.141592f;

	template<typename T>
	struct Vector2 {
		T x, y;
		Vector2()			: x(0), y(0)				{}
		Vector2(T _x, T _y) : x(_x), y(_y)	{}

		force_inline float getLength() const {
			return static_cast<float>(std::sqrt ( x * x + y * y));
		}
		force_inline Vector2<T> normalize() {
			float length = getLength();
			return { x / length, y / length };
		}
		force_inline Vector2<T> operator+(const Vector2<T>& other) {
			return { x + other.x, y + other.y };
		}
		force_inline Vector2<T> operator-(const Vector2<T>& other) {
			return { x - other.x, y - other.y };
		}
		force_inline Vector2<T> operator*(const Vector2<T>& other) {
			return { x * other.x, y * other.y };
		}
		force_inline Vector2<T> operator/(const Vector2<T>& other) {
			return { x / other.x, y / other.y };
		}

	};

	using Vector2f = Vector2<float>;
	using Vector2i = Vector2<int>;
	using Vector2u = Vector2<unsigned int>;

}