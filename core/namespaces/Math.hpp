#pragma once
#include <cmath>

using		INT			= int;
using		FLOAT		= float;
using		STRING		= const char*;
using		BOOL		= bool;

using		U_INT		= unsigned int;

namespace Math {

	constexpr FLOAT		PI		= 3.141592f;
	constexpr FLOAT		EPSILON = 0.1f;

	template<typename T>
	struct Vector2 {

		T x, y;

		Vector2(T _x, T _y) : x(_x), y(_y)	{}
		Vector2()			: x(0), y(0)	{}

		Vector2<T>		normalize()							const {
			float length = std::sqrt(x * x + y * y);
			return { x / length, y / length };
		}
		Vector2<T>		operator+(const Vector2<T>& other)	const {
			return { x + other.x, y + other.y };
		}
		Vector2<T>		operator-(const Vector2<T>& other)	const {
			return { x - other.x, y - other.y };
		}
		Vector2<T>		operator*(const Vector2<T>& other)	const {
			return { x * other.x, y * other.y };
		}
		Vector2<T>		operator/(const Vector2<T>& other)	const {
			return { x / other.x, y / other.y };
		}
		BOOL			operator==(const Vector2<T>& other) const {
			return x == other.x and y == other.y;
		}

	};

	using		Vector2f = Vector2<FLOAT>;
	using		Vector2u = Vector2<U_INT>;
	using		Vector2i = Vector2<INT>;

};