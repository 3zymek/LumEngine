#pragma once

#include "core/types.hpp"

namespace lum {

	template<cArithmetic tType>
	struct TVector2 {

		constexpr TVector2( ) : x( tType( 0 ) ), y( tType( 0 ) ) { }
		constexpr TVector2( tType x, tType y ) : x( x ), y( y ) { }
		constexpr explicit TVector2( tType scalar ) : x( scalar ), y( scalar ) { }

		template<cArithmetic tOther>
		explicit operator TVector2<tOther>( ) const {
			return { static_cast< tOther >(x), static_cast< tOther >(y) };
		}

		union {
			struct { tType x, y; };
			struct { tType r, g; };
		};

		// Arithmetic
		TVector2 operator+( const TVector2& rhs ) const { return { x + rhs.x, y + rhs.y }; }
		TVector2 operator-( const TVector2& rhs ) const { return { x - rhs.x, y - rhs.y }; }
		TVector2 operator*( const TVector2& rhs ) const { return { x * rhs.x, y * rhs.y }; }
		TVector2 operator/( const TVector2& rhs ) const { return { x / rhs.x, y / rhs.y }; }

		// Scalar
		TVector2 operator*( tType scalar ) const { return { x * scalar, y * scalar }; }
		TVector2& operator*=( tType scalar ) { x *= scalar; y *= scalar; return *this; }
		TVector2 operator/( tType scalar ) const { tType inv = tType( 1 ) / scalar; return { x * inv, y * inv }; }
		TVector2& operator/=( tType scalar ) { tType inv = tType( 1 ) / scalar; x *= inv; y *= inv; return *this; }

		// Assignment
		TVector2& operator+=( const TVector2& rhs ) { x += rhs.x; y += rhs.y; return *this; }
		TVector2& operator-=( const TVector2& rhs ) { x -= rhs.x; y -= rhs.y; return *this; }

		// Unary
		TVector2 operator-( ) const { return { -x, -y }; }

		// Comparison
		bool operator==( const TVector2& rhs ) const { return x == rhs.x && y == rhs.y; }
		bool operator!=( const TVector2& rhs ) const { return !(*this == rhs); }

		const float32* Data( ) const { return &x; }

	};

	// Scalar * Vector2
	template<cArithmetic tType>
	TVector2<tType> operator*( tType scalar, const TVector2<tType>& v ) { return v * scalar; }




	//-----------------------------------------
	// Other operations
	//-----------------------------------------

	template<cArithmetic tType>
	constexpr float32 Length( const TVector2<tType>& v ) { return sqrt( v.x * v.x + v.y * v.y ); }

	template<cArithmetic tType>
	constexpr float32 LengthSq( const TVector2<tType>& v ) { return v.x * v.x + v.y * v.y; }

	template<cArithmetic tType>
	constexpr float32 Distance( const TVector2<tType>& a, const TVector2<tType>& b ) { return Length( b - a ); }

	template<cArithmetic tType>
	constexpr TVector2<tType> Normalize( const TVector2<tType>& v ) {
		float32 invLen = 1.0f / Length( v );
		return { v.x * invLen, v.y * invLen };
	}

	template<cArithmetic tType>
	constexpr float32 Dot( const TVector2<tType>& v1, const TVector2<tType>& v2 ) { return v1.x * v2.x + v1.y * v2.y; }

	template<cArithmetic tType>
	constexpr float32 ScalarCross( const TVector2<tType>& v1, const TVector2<tType>& v2 ) { return v1.x * v2.y - v1.y * v2.x; }

	template<cArithmetic tType>
	constexpr TVector2<tType> Clamp( const TVector2<tType>& v, tType min, tType max ) {
		return {
			std::clamp( v.x, min, max ),
			std::clamp( v.y, min, max )
		};
	}

	template<cArithmetic tType>
	constexpr TVector2<tType> Abs( const TVector2<tType>& v ) {
		return { std::abs( v.x ), std::abs( v.y ) };
	}

	using Vector2 = TVector2<float32>;
	using Vector2u = TVector2<uint32>;

} // namespace lum