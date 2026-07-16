#pragma once

#include "Core/Types.hpp"

namespace lum {

	template<cArithmetic tType>
	struct TVector2 {

		constexpr TVector2( ) : mX( tType( 0 ) ), mY( tType( 0 ) ) { }
		constexpr TVector2( tType x, tType y ) : mX( x ), mY( y ) { }
		constexpr explicit TVector2( tType scalar ) : mX( scalar ), mY( scalar ) { }

		template<cArithmetic tOther>
		explicit operator TVector2<tOther>( ) const {
			return { static_cast< tOther >(mX), static_cast< tOther >(mY) };
		}

		union {
			struct { tType mX, mY; };
			struct { tType mR, mG; };
		};

		// Arithmetic
		TVector2 operator+( const TVector2& rhs ) const { return { mX + rhs.mX, mY + rhs.mY }; }
		TVector2 operator-( const TVector2& rhs ) const { return { mX - rhs.mX, mY - rhs.mY }; }
		TVector2 operator*( const TVector2& rhs ) const { return { mX * rhs.mX, mY * rhs.mY }; }
		TVector2 operator/( const TVector2& rhs ) const { return { mX / rhs.mX, mY / rhs.mY }; }

		// Scalar
		TVector2 operator+( tType scalar ) const { return { mX + scalar, mY + scalar }; }
		TVector2& operator+=( tType scalar ) { mX += scalar, mY += scalar; return *this; }
		TVector2 operator-( tType scalar ) const { return { mX - scalar, mY - scalar }; }
		TVector2& operator-=( tType scalar ) { mX -= scalar, mY -= scalar; return *this; }
		TVector2 operator*( tType scalar ) const { return { mX * scalar, mY * scalar }; }
		TVector2& operator*=( tType scalar ) { mX *= scalar; mY *= scalar; return *this; }
		TVector2 operator/( tType scalar ) const { tType inv = tType( 1 ) / scalar; return { mX * inv, mY * inv }; }
		TVector2& operator/=( tType scalar ) { tType inv = tType( 1 ) / scalar; mX *= inv; mY *= inv; return *this; }


		// Assignment
		TVector2& operator+=( const TVector2& rhs ) { mX += rhs.mX; mY += rhs.mY; return *this; }
		TVector2& operator-=( const TVector2& rhs ) { mX -= rhs.mX; mY -= rhs.mY; return *this; }

		// Unary
		TVector2 operator-( ) const { return { -mX, -mY }; }

		// Comparison
		bool operator==( const TVector2& rhs ) const { return mX == rhs.mX && mY == rhs.mY; }
		bool operator!=( const TVector2& rhs ) const { return !(*this == rhs); }

		const float32* Data( ) const { return &mX; }

	};

	// Scalar * Vector2
	template<cArithmetic tType>
	TVector2<tType> operator*( tType scalar, const TVector2<tType>& v ) { return v * scalar; }




	//-----------------------------------------
	// Other operations
	//-----------------------------------------

	template<cArithmetic tType>
	float32 Length( const TVector2<tType>& v ) { return sqrt( v.mX * v.mX + v.mY * v.mY ); }

	template<cArithmetic tType>
	constexpr float32 LengthSq( const TVector2<tType>& v ) { return v.mX * v.mX + v.mY * v.mY; }

	template<cArithmetic tType>
	float32 Distance( const TVector2<tType>& a, const TVector2<tType>& b ) { return Length( b - a ); }

	template<cArithmetic tType>
	TVector2<tType> Normalize( const TVector2<tType>& v ) {
		float32 invLen = 1.0f / Length( v );
		return { v.mX * invLen, v.mY * invLen };
	}

	template<cArithmetic tType>
	constexpr float32 Dot( const TVector2<tType>& v1, const TVector2<tType>& v2 ) { return v1.mX * v2.mX + v1.mY * v2.mY; }

	template<cArithmetic tType>
	constexpr float32 ScalarCross( const TVector2<tType>& v1, const TVector2<tType>& v2 ) { return v1.mX * v2.mY - v1.mY * v2.mX; }

	template<cArithmetic tType>
	constexpr TVector2<tType> Clamp( const TVector2<tType>& v, tType min, tType max ) {
		return {
			std::clamp( v.mX, min, max ),
			std::clamp( v.mY, min, max )
		};
	}

	template<cArithmetic tType>
	TVector2<tType> Abs( const TVector2<tType>& v ) {
		return { std::abs( v.mX ), std::abs( v.mY ) };
	}

	using Vector2 = TVector2<float32>;
	using Vector2u = TVector2<uint32>;

} // namespace lum