#pragma once

#include "Core/Types.hpp"

namespace lum {

	struct Quaternion {

		constexpr Quaternion( float32 x, float32 y, float32 z, float32 w ) : mX( x ), mY( y ), mZ( z ), mW( w ) { }
		constexpr explicit Quaternion( float32 scalar ) : mX( scalar ), mY( scalar ), mZ( scalar ), mW(scalar) { }

		float32 mX{}, mY{}, mZ{}, mW{};

		// Arithmetic
		Quaternion operator+( const Quaternion& rhs ) const { return { mX + rhs.mX, mY + rhs.mY, mZ + rhs.mZ, mW + rhs.mW }; }
		Quaternion operator-( const Quaternion& rhs ) const { return { mX - rhs.mX, mY - rhs.mY, mZ - rhs.mZ, mW - rhs.mW }; }
		Quaternion operator*( const Quaternion& rhs ) const { 
			return {
				mW * rhs.mX + mX * rhs.mW + mY * rhs.mZ - mZ * rhs.mY,
				mW * rhs.mY - mX * rhs.mZ + mY * rhs.mW + mZ * rhs.mX,
				mW * rhs.mZ + mX * rhs.mY - mY * rhs.mX + mZ * rhs.mW,
				mW * rhs.mW - mX * rhs.mX - mY * rhs.mY - mZ * rhs.mZ
			};
		}
		Quaternion operator/( const Quaternion& rhs ) const { return { mX / rhs.mX, mY / rhs.mY, mZ / rhs.mZ, mW / rhs.mW }; }

		// Scalar
		template<cArithmetic tType>
		Quaternion operator*( tType scalar ) const { return { mX * scalar, mY * scalar, mZ * scalar, mW * scalar }; }
		template<cArithmetic tType>
		Quaternion& operator*=( tType scalar ) { mX *= scalar; mY *= scalar; mZ *= scalar; mW *= scalar; return *this; }
		template<cArithmetic tType>
		Quaternion operator/( tType scalar ) const { tType inv = tType( 1 ) / scalar; return { mX * inv, mY * inv, mZ * inv, mW * inv }; }
		template<cArithmetic tType>
		Quaternion& operator/=( tType scalar ) { tType inv = tType( 1 ) / scalar; mX *= inv; mY *= inv; mZ *= inv; mW *= inv; return *this; }

		// Assignment
		Quaternion& operator+=( const Quaternion& rhs ) { mX += rhs.mX; mY += rhs.mY; mZ += rhs.mZ; mW += rhs.mW; return *this; }
		Quaternion& operator-=( const Quaternion& rhs ) { mX -= rhs.mX; mY -= rhs.mY; mZ -= rhs.mZ; mW -= rhs.mW; return *this; }

		// Unary
		Quaternion operator-( ) const { return { -mX, -mY, -mZ, -mW }; }

		// Comparison
		bool operator==( const Quaternion& rhs ) const { return mX == rhs.mX && mY == rhs.mY && mZ == rhs.mZ && mW == rhs.mW; }
		bool operator!=( const Quaternion& rhs ) const { return !(*this == rhs); }

		const float32* Data( ) const { return &mX; }

	};

	// Scalar * Vector2
	template<cArithmetic tType>
	Quaternion operator*( tType scalar, const Quaternion& q ) { return q * scalar; }



	//-----------------------------------------
	// Other operations
	//-----------------------------------------

	inline float32 Length( const Quaternion& q ) { return sqrt( q.mX * q.mX + q.mY * q.mY + q.mZ * q.mZ + q.mW * q.mW ); }

	inline float32 LengthSq( const Quaternion& q ) { return q.mX * q.mX + q.mY * q.mY + q.mZ * q.mZ + q.mW * q.mW; }

	inline Quaternion Normalize( const Quaternion& q ) {
		float32 invLen = 1.0f / Length( q );
		return { q.mX * invLen, q.mY * invLen, q.mZ * invLen, q.mW * invLen };
	}

	inline float32 Dot( const Quaternion& q1, const Quaternion& q2 ) { return q1.mX * q2.mX + q1.mY * q2.mY + q1.mZ * q2.mZ + q1.mW * q2.mW; }

	inline Quaternion Abs( const Quaternion& q ) {
		return { std::abs( q.mX ), std::abs( q.mY ), std::abs( q.mZ ), std::abs( q.mW ) };
	}

	inline Quaternion Conjugate( const Quaternion& q ) {
		return { -q.mX, -q.mY, -q.mZ, q.mW };
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
		return FromEuler( v.mY, v.mX, v.mZ );
	}

} // namespace lum