#pragma once

#include "core/types.hpp"

namespace lum {

	template<cArithmetic tType>
	struct TVector3 {

		constexpr TVector3( ) : mX( tType( 0 ) ), mY( tType( 0 ) ), mZ( tType( 0 ) ) { }
		constexpr TVector3( tType x, tType y, tType z ) : mX( x ), mY( y ), mZ( z ) { }
		constexpr explicit TVector3( tType scalar ) : mX( scalar ), mY( scalar ), mZ( scalar ) { }

		template<cArithmetic tOther>
		explicit operator TVector3<tOther>( ) const {
			return { static_cast< tOther >(mX), static_cast< tOther >(mY), static_cast< tOther >(mZ) };
		}

		union {
			struct { tType mX{}, mY{}, mZ{}; };
			struct { tType mR{}, mG{}, mB{}; };
		};

		// Arithmetic
		TVector3 operator+( const TVector3& rhs ) const { return { mX + rhs.mX, mY + rhs.mY, mZ + rhs.mZ }; }
		TVector3 operator-( const TVector3& rhs ) const { return { mX - rhs.mX, mY - rhs.mY, mZ - rhs.mZ }; }
		TVector3 operator*( const TVector3& rhs ) const { return { mX * rhs.mX, mY * rhs.mY, mZ * rhs.mZ }; }
		TVector3 operator/( const TVector3& rhs ) const { return { mX / rhs.mX, mY / rhs.mY, mZ / rhs.mZ }; }

		// Scalar
		TVector3 operator+( tType scalar ) const { return { mX + scalar, mY + scalar, mZ + scalar }; }
		TVector3& operator+=( tType scalar ) { mX += scalar, mY += scalar; mZ += scalar; return *this; }
		TVector3 operator-( tType scalar ) const { return { mX - scalar, mY - scalar, mZ - scalar }; }
		TVector3& operator-=( tType scalar ) { mX -= scalar, mY -= scalar; mZ -= scalar; return *this; }
		TVector3 operator*( tType scalar ) const { return { mX * scalar, mY * scalar, mZ * scalar }; }
		TVector3& operator*=( tType scalar ) { mX *= scalar; mY *= scalar; mZ *= scalar; return *this; }
		TVector3 operator/( tType scalar ) const { tType inv = tType( 1 ) / scalar; return { mX * inv, mY * inv, mZ * inv }; }
		TVector3& operator/=( tType scalar ) { tType inv = tType( 1 ) / scalar; mX *= inv; mY *= inv; mZ *= inv; return *this; }

		// Assignment
		TVector3& operator+=( const TVector3& rhs ) { mX += rhs.mX; mY += rhs.mY; mZ += rhs.mZ; return *this; }
		TVector3& operator-=( const TVector3& rhs ) { mX -= rhs.mX; mY -= rhs.mY; mZ -= rhs.mZ; return *this; }

		// Unary
		TVector3 operator-( ) const { return { -mX, -mY, -mZ }; }

		// Comparison
		bool operator==( const TVector3& rhs ) const { return mX == rhs.mX && mY == rhs.mY && mZ == rhs.mZ; }
		bool operator!=( const TVector3& rhs ) const { return !(*this == rhs); }

		tType* Data( ) { return &mX; }

	};

	// Scalar * Vector2
	template<cArithmetic tType>
	TVector3<tType> operator*( tType scalar, const TVector3<tType>& v ) { return v * scalar; }





	//-----------------------------------------
	// Other operations
	//-----------------------------------------

	template<cArithmetic tType>
	float32 Length( const TVector3<tType>& v ) { return sqrt( v.mX * v.mX + v.mY * v.mY + v.mZ * v.mZ ); }

	template<cArithmetic tType>
	constexpr float32 LengthSq( const TVector3<tType>& v ) { return v.mX * v.mX + v.mY * v.mY + v.mZ * v.mZ; }

	template<cArithmetic tType>
	float32 Distance( const TVector3<tType>& a, const TVector3<tType>& b ) { return Length( b - a ); }

	template<cArithmetic tType>
	TVector3<tType> Normalize( const TVector3<tType>& v ) {
		float32 invLen = 1.0f / Length( v );
		return { v.mX * invLen, v.mY * invLen, v.mZ * invLen };
	}

	template<cArithmetic tType>
	float32 Dot( const TVector3<tType>& v1, const TVector3<tType>& v2 ) { return v1.mX * v2.mX + v1.mY * v2.mY + v1.mZ * v2.mZ; }

	template<cArithmetic tType>
	constexpr TVector3<tType> Cross( const TVector3<tType>& v1, const TVector3<tType>& v2 ) {
		return {
			v1.mY * v2.mZ - v1.mZ * v2.mY,
			v1.mZ * v2.mX - v1.mX * v2.mZ,
			v1.mX * v2.mY - v1.mY * v2.mX
		};
	}

	template<cArithmetic tType>
	constexpr TVector3<tType> Clamp( const TVector3<tType>& v, tType min, tType max ) {
		return {
			std::clamp( v.mX, min, max ),
			std::clamp( v.mY, min, max ),
			std::clamp( v.mZ, min, max )
		};
	}

	template<cArithmetic tType>
	TVector3<tType> Abs( const TVector3<tType>& v ) {
		return { std::abs( v.mX ), std::abs( v.mY ), std::abs( v.mZ ) };
	}

	using Vector3 = TVector3<float32>;
	using Vector3u = TVector3<uint32>;

} // namespace lum