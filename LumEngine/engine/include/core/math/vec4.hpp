#pragma once

#include "core/types.hpp"

namespace lum {

	template<cArithmetic tType>
	struct TVector4 {

		constexpr TVector4( ) : x( tType( 0 ) ), y( tType( 0 ) ), z( tType( 0 ) ), w( tType( 0 ) ) { }
		constexpr TVector4( tType x, tType y, tType z, tType w ) : x( x ), y( y ), z( z ), w( w ) { }
		constexpr explicit TVector4( tType scalar ) : x( scalar ), y( scalar ), z( scalar ), w(scalar) { }
		template<cArithmetic tOther>
		constexpr TVector4( const TVector3<tOther>& v, tOther w )
			: x( static_cast< tType >(v.x) ),
			y( static_cast< tType >(v.y) ),
			z( static_cast< tType >(v.z) ),
			w( static_cast< tType >(w) ) { }

		template<cArithmetic tOther>
		explicit operator TVector4<tOther>( ) const {
			return { static_cast< tOther >(x), static_cast< tOther >(y), static_cast< tOther >(z), static_cast< tOther >(w) };
		}
		template<cArithmetic tOther>
		explicit operator TVector3<tOther>( ) const {
			return { static_cast< tOther >(x), static_cast< tOther >(y), static_cast< tOther >(z) };
		}
		template<cArithmetic tOther>
		explicit operator TVector2<tOther>( ) const {
			return { static_cast< tOther >(x), static_cast< tOther >(y) };
		}

		union {
			struct { tType x, y, z, w; };
			struct { tType r, g, b, a; };
		};

		// Arithmetic
		TVector4 operator+( const TVector4& rhs ) const { return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w }; }
		TVector4 operator-( const TVector4& rhs ) const { return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w }; }
		TVector4 operator*( const TVector4& rhs ) const { return { x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w }; }
		TVector4 operator/( const TVector4& rhs ) const { return { x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w }; }

		// Scalar
		TVector4 operator+( tType scalar ) const { return { x + scalar, y + scalar, z + scalar, w + scalar }; }
		TVector4& operator+=( tType scalar ) { x += scalar, y += scalar; z += scalar; w += scalar; return *this; }
		TVector4 operator-( tType scalar ) const { return { x - scalar, y - scalar, z - scalar, w - scalar }; }
		TVector4& operator-=( tType scalar ) { x -= scalar, y -= scalar; z -= scalar; w -= scalar; return *this; }
		TVector4 operator*( tType scalar ) const { return { x * scalar, y * scalar, z * scalar, w * scalar }; }
		TVector4& operator*=( tType scalar ) { x *= scalar; y *= scalar; z *= scalar; w *= scalar; return *this; }
		TVector4 operator/( tType scalar ) const { tType inv = tType( 1 ) / scalar; return { x * inv, y * inv, z * inv, w * inv }; }
		TVector4& operator/=( tType scalar ) { tType inv = tType( 1 ) / scalar; x *= inv; y *= inv; z *= inv; w *= inv; return *this; }

		// Assignment
		TVector4& operator+=( const TVector4& rhs ) { x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this; }
		TVector4& operator-=( const TVector4& rhs ) { x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this; }

		// Unary
		TVector4 operator-( ) const { return { -x, -y, -z, -w }; }

		// Comparison
		bool operator==( const TVector4& rhs ) const { return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w; }
		bool operator!=( const TVector4& rhs ) const { return !(*this == rhs); }

		const float32* Data( ) const { return &x; }

	};

	// Scalar * Vector2
	template<cArithmetic tType>
	TVector4<tType> operator*( tType scalar, const TVector4<tType>& v ) { return v * scalar; }



	//-----------------------------------------
	// Other operations
	//-----------------------------------------

	template<cArithmetic tType>
	constexpr float32 Length( const TVector4<tType>& v ) { return sqrt( v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w ); }

	template<cArithmetic tType>
	constexpr float32 LengthSq( const TVector4<tType>& v ) { return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w; }

	template<cArithmetic tType>
	constexpr float32 Distance( const TVector4<tType>& a, const TVector4<tType>& b ) { return Length( b - a ); }

	template<cArithmetic tType>
	constexpr TVector4<tType> Normalize( const TVector4<tType>& v ) {
		float32 invLen = 1.0f / Length( v );
		return { v.x * invLen, v.y * invLen, v.z * invLen, v.w * invLen };
	}

	template<cArithmetic tType>
	constexpr float32 Dot( const TVector4<tType>& v1, const TVector4<tType>& v2 ) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w; }

	template<cArithmetic tType>
	constexpr TVector4<tType> Clamp( const TVector4<tType>& v, tType min, tType max ) {
		return {
			std::clamp( v.x, min, max ),
			std::clamp( v.y, min, max ),
			std::clamp( v.z, min, max ),
			std::clamp( v.w, min, max )
		};
	}

	template<cArithmetic tType>
	constexpr TVector4<tType> Abs( const TVector4<tType>& v ) {
		return { std::abs( v.x ), std::abs( v.y ), std::abs( v.z ), std::abs( v.w ) };
	}

	using Vector4 = TVector4<float32>;
	using Vector4u = TVector4<uint32>;

} // namespace lum