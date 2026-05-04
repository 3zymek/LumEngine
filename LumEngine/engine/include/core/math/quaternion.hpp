#pragma once

#include "core/types.hpp"

namespace lum {

	struct Quaternion {

		constexpr Quaternion( float32 x, float32 y, float32 z, float32 w ) : x( x ), y( y ), z( z ), w( w ) { }
		constexpr explicit Quaternion( float32 scalar ) : x( scalar ), y( scalar ), z( scalar ), w(scalar) { }

		float32 x{};
		float32 y{};
		float32 z{};
		float32 w{};

		// Arithmetic
		Quaternion operator+( const Quaternion& rhs ) const { return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w }; }
		Quaternion operator-( const Quaternion& rhs ) const { return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w }; }
		Quaternion operator*( const Quaternion& rhs ) const { 
			return {
				w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
				w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
				w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w,
				w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z
			};
		}
		Quaternion operator/( const Quaternion& rhs ) const { return { x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w }; }

		// Scalar
		template<cArithmetic tType>
		Quaternion operator*( tType scalar ) const { return { x * scalar, y * scalar, z * scalar, w * scalar }; }
		template<cArithmetic tType>
		Quaternion& operator*=( tType scalar ) { x *= scalar; y *= scalar; z *= scalar; w *= scalar; return *this; }
		template<cArithmetic tType>
		Quaternion operator/( tType scalar ) const { tType inv = tType( 1 ) / scalar; return { x * inv, y * inv, z * inv, w * inv }; }
		template<cArithmetic tType>
		Quaternion& operator/=( tType scalar ) { tType inv = tType( 1 ) / scalar; x *= inv; y *= inv; z *= inv; w *= inv; return *this; }

		// Assignment
		Quaternion& operator+=( const Quaternion& rhs ) { x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this; }
		Quaternion& operator-=( const Quaternion& rhs ) { x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this; }

		// Unary
		Quaternion operator-( ) const { return { -x, -y, -z, -w }; }

		// Comparison
		bool operator==( const Quaternion& rhs ) const { return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w; }
		bool operator!=( const Quaternion& rhs ) const { return !(*this == rhs); }

		const float32* Data( ) const { return &x; }

	};

	// Scalar * Vector2
	template<cArithmetic tType>
	Quaternion operator*( tType scalar, const Quaternion& q ) { return q * scalar; }



	//-----------------------------------------
	// Other operations
	//-----------------------------------------

	inline float32 Length( const Quaternion& q ) { return sqrt( q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w ); }

	inline float32 LengthSq( const Quaternion& q ) { return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w; }

	inline Quaternion Normalize( const Quaternion& q ) {
		float32 invLen = 1.0f / Length( q );
		return { q.x * invLen, q.y * invLen, q.z * invLen, q.w * invLen };
	}

	inline float32 Dot( const Quaternion& q1, const Quaternion& q2 ) { return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w; }

	inline Quaternion Abs( const Quaternion& q ) {
		return { std::abs( q.x ), std::abs( q.y ), std::abs( q.z ), std::abs( q.w ) };
	}

	inline Quaternion Conjugate( const Quaternion& q ) {
		return { -q.x, -q.y, -q.z, q.w };
	}
	inline Quaternion Inverse( const Quaternion& q ) {
		return Conjugate( q ) / LengthSq( q );
	}

	inline Quaternion FromEuler( float32 pitch, float32 roll, float32 yaw ) {
		float32 cp = cos( pitch * 0.5f ), sp = sin( pitch * 0.5f );
		float32 cy = cos( yaw * 0.5f ), sy = sin( yaw * 0.5f );
		float32 cr = cos( roll * 0.5f ), sr = sin( roll * 0.5f );
		return {
			sr * cp * cy - cr * sp * sy,
			cr * sp * cy + sr * cp * sy,
			cr * cp * sy - sr * sp * cy,
			cr * cp * cy + sr * sp * sy
		};
	}
	inline Quaternion FromEuler( const Vector3& v ) {
		return FromEuler( v.y, v.x, v.z );
	}

} // namespace lum