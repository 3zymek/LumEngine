#pragma once

#include "core/types.hpp"

namespace lum {

	struct Matrix4 {

		constexpr Matrix4() { }
		constexpr explicit Matrix4( float32 scalar ) {
			mData[ 0 ] = scalar;
			mData[ 5 ] = scalar;
			mData[ 10 ] = scalar;
			mData[ 15 ] = scalar;
		}

		float32 mData[ 16 ]{};

		// Arithmetic
		Matrix4 operator+( const Matrix4& rhs ) const {
			Matrix4 result;
			for (uint32 i = 0; i < 16; i++)
				result.mData[ i ] = mData[ i ] + rhs.mData[ i ];
			return result;
	   }
		Matrix4 operator-( const Matrix4& rhs ) const {
			Matrix4 result;
			for (uint32 i = 0; i < 16; i++)
				result.mData[ i ] = mData[ i ] - rhs.mData[ i ];
			return result;
		}
		Matrix4 operator*( const Matrix4& rhs ) const {
			Matrix4 result;
			for (uint32 col = 0; col < 4; col++)
				for (uint32 row = 0; row < 4; row++)
					for (uint32 k = 0; k < 4; k++)
						result.mData[ col * 4 + row ] += mData[ k * 4 + row ] * rhs.mData[ col * 4 + k ];
			return result;
		}
		Matrix4 operator/( const Matrix4& rhs ) const {
			Matrix4 result;
			for (uint32 i = 0; i < 16; i++)
				result.mData[ i ] = mData[ i ] / rhs.mData[ i ];
			return result;
		}

		// Scalar
		template<cArithmetic tType>
		Matrix4 operator*( tType scalar ) {
			Matrix4 result;
			for (uint32 i = 0; i < 16; i++)
				result.mData[ i ] = mData[i] * scalar;
			return result;
		}
		template<cArithmetic tType>
		Matrix4& operator*=( tType scalar ) {
			for (uint32 i = 0; i < 16; i++)
				mData[ i ] *= scalar;
			return *this;
		}
		template<cArithmetic tType>
		Matrix4 operator/( tType scalar ) {
			Matrix4 result;
			tType inv = 1.0 / scalar;
			for (uint32 i = 0; i < 16; i++)
				result.mData[ i ] = mData[ i ] * inv;
			return result;
		}
		template<cArithmetic tType>
		Matrix4& operator/=( tType scalar ) {
			tType inv = 1.0 / scalar;
			for (uint32 i = 0; i < 16; i++)
				mData[ i ] *= inv;
			return *this;
		}

		// Assignment
		Matrix4& operator+=( const Matrix4& rhs ) {
			for (uint32 i = 0; i < 16; i++)
				mData[ i ] += rhs.mData[ i ];
			return *this;
		}
		Matrix4& operator-=( const Matrix4& rhs ) {
			for (uint32 i = 0; i < 16; i++)
				mData[ i ] -= rhs.mData[ i ];
			return *this;
		}
		Matrix4& operator*=( const Matrix4& rhs ) {
			*this = *this * rhs;
			return *this;
		}
		Matrix4& operator/=( const Matrix4& rhs ) {
			for (uint32 i = 0; i < 16; i++)
				mData[ i ] /= rhs.mData[ i ];
			return *this;
		}
	    
		// Unary
		Matrix4 operator-( ) const {
			Matrix4 result;
			for (uint32 i = 0; i < 16; i++)
				result.mData[ i ] = -mData[ i ];
			return result;
		}

		// Comparison
		bool operator==( const Matrix4& rhs ) const {
			for (uint32 i = 0; i < 16; i++) {
				if (mData[ i ] != rhs.mData[ i ]) return false;
			}
			return true;
		}
		bool operator!=( const Matrix4& rhs ) const { return !(*this == rhs); }

		const float32* Data( ) const { return &mData[ 0 ]; }

		float32* operator[]( uint32 col ) { return &mData[ col * 4 ]; }
		const float32* operator[]( uint32 col ) const { return &mData[ col * 4 ]; }

	};

}