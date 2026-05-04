#pragma once

#include "core/types.hpp"

namespace lum {

	template<cArithmetic tType>
	struct TVector3 {

		constexpr TVector3( ) : x( tType( 0 ) ), y( tType( 0 ) ), z( tType( 0 ) ) { }
		constexpr TVector3( tType x, tType y, tType z ) : x( x ), y( y ), z( z ) { }
		constexpr explicit TVector3( tType scalar ) : x( scalar ), y( scalar ), z( scalar ) { }

		template<cArithmetic tOther>
		explicit operator TVector3<tOther>( ) const {
			return { static_cast< tOther >(x), static_cast< tOther >(y), static_cast< tOther >(z) };
		}

		union {
			struct { tType x, y, z; };
			struct { tType r, g, b; };
		};

		// Arithmetic
		TVector3 operator+( const TVector3& rhs ) const { return { x + rhs.x, y + rhs.y, z + rhs.z }; }
		TVector3 operator-( const TVector3& rhs ) const { return { x - rhs.x, y - rhs.y, z - rhs.z }; }
		TVector3 operator*( const TVector3& rhs ) const { return { x * rhs.x, y * rhs.y, z * rhs.z }; }
		TVector3 operator/( const TVector3& rhs ) const { return { x / rhs.x, y / rhs.y, z / rhs.z }; }

		// Scalar
		TVector3 operator*( tType scalar ) const { return { x * scalar, y * scalar, z * scalar }; }
		TVector3& operator*=( tType scalar ) { x *= scalar; y *= scalar; z *= scalar; return *this; }
		TVector3 operator/( tType scalar ) const { tType inv = tType( 1 ) / scalar; return { x * inv, y * inv, z * inv }; }
		TVector3& operator/=( tType scalar ) { tType inv = tType( 1 ) / scalar; x *= inv; y *= inv; z *= inv; return *this; }

		// Assignment
		TVector3& operator+=( const TVector3& rhs ) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
		TVector3& operator-=( const TVector3& rhs ) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }

		// Unary
		TVector3 operator-( ) const { return { -x, -y, -z }; }

		// Comparison
		bool operator==( const TVector3& rhs ) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
		bool operator!=( const TVector3& rhs ) const { return !(*this == rhs); }

		tType* Data( ) { return &x; }

	};

	// Scalar * Vector2
	template<cArithmetic tType>
	TVector3<tType> operator*( tType scalar, const TVector3<tType>& v ) { return v * scalar; }





	//-----------------------------------------
	// Other operations
	//-----------------------------------------

	template<cArithmetic tType>
	float32 Length( const TVector3<tType>& v ) { return sqrt( v.x * v.x + v.y * v.y + v.z * v.z ); }

	template<cArithmetic tType>
	constexpr float32 LengthSq( const TVector3<tType>& v ) { return v.x * v.x + v.y * v.y + v.z * v.z; }

	template<cArithmetic tType>
	float32 Distance( const TVector3<tType>& a, const TVector3<tType>& b ) { return Length( b - a ); }

	template<cArithmetic tType>
	TVector3<tType> Normalize( const TVector3<tType>& v ) {
		float32 invLen = 1.0f / Length( v );
		return { v.x * invLen, v.y * invLen, v.z * invLen };
	}

	template<cArithmetic tType>
	float32 Dot( const TVector3<tType>& v1, const TVector3<tType>& v2 ) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

	template<cArithmetic tType>
	constexpr TVector3<tType> Cross( const TVector3<tType>& v1, const TVector3<tType>& v2 ) {
		return {
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x
		};
	}

	template<cArithmetic tType>
	constexpr TVector3<tType> Clamp( const TVector3<tType>& v, tType min, tType max ) {
		return {
			std::clamp( v.x, min, max ),
			std::clamp( v.y, min, max ),
			std::clamp( v.z, min, max )
		};
	}

	template<cArithmetic tType>
	TVector3<tType> Abs( const TVector3<tType>& v ) {
		return { std::abs( v.x ), std::abs( v.y ), std::abs( v.z ) };
	}

	using Vector3 = TVector3<float32>;
	using Vector3u = TVector3<uint32>;

} // namespace lum