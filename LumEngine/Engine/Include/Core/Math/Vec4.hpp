#pragma once

#include "Core/Types.hpp"

namespace lum {

	template<cArithmetic tType>
	struct TVector4 {

		constexpr TVector4( ) : mX( tType( 0 ) ), mY( tType( 0 ) ), mZ( tType( 0 ) ), mW( tType( 0 ) ) { }
		constexpr TVector4( tType x, tType y, tType z, tType w ) : mX( x ), mY( y ), mZ( z ), mW( w ) { }
		constexpr explicit TVector4( tType scalar ) : mX( scalar ), mY( scalar ), mZ( scalar ), mW(scalar) { }
		template<cArithmetic tOther>
		constexpr TVector4( const TVector3<tOther>& v, tOther w )
			: mX( static_cast< tType >(v.mX) ),
			mY( static_cast< tType >(v.mY) ),
			mZ( static_cast< tType >(v.mZ) ),
			mW( static_cast< tType >(w) ) { }

		template<cArithmetic tOther>
		explicit operator TVector4<tOther>( ) const {
			return { static_cast< tOther >(mX), static_cast< tOther >(mY), static_cast< tOther >(mZ), static_cast< tOther >(mW) };
		}
		template<cArithmetic tOther>
		explicit operator TVector3<tOther>( ) const {
			return { static_cast< tOther >(mX), static_cast< tOther >(mY), static_cast< tOther >(mZ) };
		}
		template<cArithmetic tOther>
		explicit operator TVector2<tOther>( ) const {
			return { static_cast< tOther >(mX), static_cast< tOther >(mY) };
		}

		union {
			struct { tType mX, mY, mZ, mW; };
			struct { tType mR, mG, mB, mA; };
		};

		// Arithmetic
		TVector4 operator+( const TVector4& rhs ) const { return { mX + rhs.mX, mY + rhs.mY, mZ + rhs.mZ, mW + rhs.mW }; }
		TVector4 operator-( const TVector4& rhs ) const { return { mX - rhs.mX, mY - rhs.mY, mZ - rhs.mZ, mW - rhs.mW }; }
		TVector4 operator*( const TVector4& rhs ) const { return { mX * rhs.mX, mY * rhs.mY, mZ * rhs.mZ, mW * rhs.mW }; }
		TVector4 operator/( const TVector4& rhs ) const { return { mX / rhs.mX, mY / rhs.mY, mZ / rhs.mZ, mW / rhs.mW }; }

		// Scalar
		TVector4 operator+( tType scalar ) const { return { mX + scalar, mY + scalar, mZ + scalar, mW + scalar }; }
		TVector4& operator+=( tType scalar ) { mX += scalar, mY += scalar; mZ += scalar; mW += scalar; return *this; }
		TVector4 operator-( tType scalar ) const { return { mX - scalar, mY - scalar, mZ - scalar, mW - scalar }; }
		TVector4& operator-=( tType scalar ) { mX -= scalar, mY -= scalar; mZ -= scalar; mW -= scalar; return *this; }
		TVector4 operator*( tType scalar ) const { return { mX * scalar, mY * scalar, mZ * scalar, mW * scalar }; }
		TVector4& operator*=( tType scalar ) { mX *= scalar; mY *= scalar; mZ *= scalar; mW *= scalar; return *this; }
		TVector4 operator/( tType scalar ) const { tType inv = tType( 1 ) / scalar; return { mX * inv, mY * inv, mZ * inv, mW * inv }; }
		TVector4& operator/=( tType scalar ) { tType inv = tType( 1 ) / scalar; mX *= inv; mY *= inv; mZ *= inv; mW *= inv; return *this; }

		// Assignment
		TVector4& operator+=( const TVector4& rhs ) { mX += rhs.mX; mY += rhs.mY; mZ += rhs.mZ; mW += rhs.mW; return *this; }
		TVector4& operator-=( const TVector4& rhs ) { mX -= rhs.mX; mY -= rhs.mY; mZ -= rhs.mZ; mW -= rhs.mW; return *this; }

		// Unary
		TVector4 operator-( ) const { return { -mX, -mY, -mZ, -mW }; }

		// Comparison
		bool operator==( const TVector4& rhs ) const { return mX == rhs.mX && mY == rhs.mY && mZ == rhs.mZ && mW == rhs.mW; }
		bool operator!=( const TVector4& rhs ) const { return !(*this == rhs); }

		const float32* Data( ) const { return &mX; }

	};

	// Scalar * Vector2
	template<cArithmetic tType>
	TVector4<tType> operator*( tType scalar, const TVector4<tType>& v ) { return v * scalar; }



	//-----------------------------------------
	// Other operations
	//-----------------------------------------

	template<cArithmetic tType>
	constexpr float32 Length( const TVector4<tType>& v ) { return sqrt( v.mX * v.mX + v.mY * v.mY + v.mZ * v.mZ + v.mW * v.mW ); }

	template<cArithmetic tType>
	constexpr float32 LengthSq( const TVector4<tType>& v ) { return v.mX * v.mX + v.mY * v.mY + v.mZ * v.mZ + v.mW * v.mW; }

	template<cArithmetic tType>
	constexpr float32 Distance( const TVector4<tType>& a, const TVector4<tType>& b ) { return Length( b - a ); }

	template<cArithmetic tType>
	constexpr TVector4<tType> Normalize( const TVector4<tType>& v ) {
		float32 invLen = 1.0f / Length( v );
		return { v.mX * invLen, v.mY * invLen, v.mZ * invLen, v.mW * invLen };
	}

	template<cArithmetic tType>
	constexpr float32 Dot( const TVector4<tType>& v1, const TVector4<tType>& v2 ) { return v1.mX * v2.mX + v1.mY * v2.mY + v1.mZ * v2.mZ + v1.mW * v2.mW; }

	template<cArithmetic tType>
	constexpr TVector4<tType> Clamp( const TVector4<tType>& v, tType min, tType max ) {
		return {
			std::clamp( v.mX, min, max ),
			std::clamp( v.mY, min, max ),
			std::clamp( v.mZ, min, max ),
			std::clamp( v.mW, min, max )
		};
	}

	template<cArithmetic tType>
	constexpr TVector4<tType> Abs( const TVector4<tType>& v ) {
		return { std::abs( v.mX ), std::abs( v.mY ), std::abs( v.mZ ), std::abs( v.mW ) };
	}

	using Vector4 = TVector4<float32>;
	using Vector4u = TVector4<uint32>;

} // namespace lum