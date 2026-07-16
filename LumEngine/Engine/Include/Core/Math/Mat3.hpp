#pragma once

#include "Core/Types.hpp"

namespace lum {

	struct Matrix3 {

		constexpr Matrix3( ) { }
		constexpr explicit Matrix3( float32 scalar ) {
			mData[ 0 ] = scalar;
			mData[ 4 ] = scalar;
			mData[ 8 ] = scalar;
		}

		float32 mData[ 9 ]{};

		// Arithmetic
		Matrix3 operator+( const Matrix3& rhs ) const {
			Matrix3 result;
			for (uint32 i = 0; i < 9; i++)
				result.mData[ i ] = mData[ i ] + rhs.mData[ i ];
			return result;
		}
		Matrix3 operator-( const Matrix3& rhs ) const {
			Matrix3 result;
			for (uint32 i = 0; i < 9; i++)
				result.mData[ i ] = mData[ i ] - rhs.mData[ i ];
			return result;
		}
		Matrix3 operator*( const Matrix3& rhs ) const {
			Matrix3 result;
			for (uint32 col = 0; col < 3; col++)
				for (uint32 row = 0; row < 3; row++)
					for (uint32 k = 0; k < 3; k++)
						result.mData[ col * 3 + row ] += mData[ k * 3 + row ] * rhs.mData[ col * 3 + k ];
			return result;
		}
		Matrix3 operator/( const Matrix3& rhs ) const {
			Matrix3 result;
			for (uint32 i = 0; i < 9; i++)
				result.mData[ i ] = mData[ i ] / rhs.mData[ i ];
			return result;
		}

		// Scalar
		template<cArithmetic tType>
		Matrix3 operator*( tType scalar ) {
			Matrix3 result;
			for (uint32 i = 0; i < 9; i++)
				result.mData[ i ] = mData[ i ] * scalar;
			return result;
		}
		template<cArithmetic tType>
		Matrix3& operator*=( tType scalar ) {
			for (uint32 i = 0; i < 9; i++)
				mData[ i ] *= scalar;
			return *this;
		}
		template<cArithmetic tType>
		Matrix3 operator/( tType scalar ) {
			Matrix3 result;
			tType inv = 1.0 / scalar;
			for (uint32 i = 0; i < 9; i++)
				result.mData[ i ] = mData[ i ] * inv;
			return result;
		}
		template<cArithmetic tType>
		Matrix3& operator/=( tType scalar ) {
			tType inv = 1.0 / scalar;
			for (uint32 i = 0; i < 9; i++)
				mData[ i ] *= inv;
			return *this;
		}

		// Assignment
		Matrix3& operator+=( const Matrix3& rhs ) {
			for (uint32 i = 0; i < 9; i++)
				mData[ i ] += rhs.mData[ i ];
			return *this;
		}
		Matrix3& operator-=( const Matrix3& rhs ) {
			for (uint32 i = 0; i < 9; i++)
				mData[ i ] -= rhs.mData[ i ];
			return *this;
		}
		Matrix3& operator*=( const Matrix3& rhs ) {
			*this = *this * rhs;
			return *this;
		}
		Matrix3& operator/=( const Matrix3& rhs ) {
			for (uint32 i = 0; i < 9; i++)
				mData[ i ] /= rhs.mData[ i ];
			return *this;
		}

		// Unary
		Matrix3 operator-( ) const { 
			Matrix3 result;
			for (uint32 i = 0; i < 9; i++)
				result.mData[ i ] = -mData[ i ];
			return result;
		}

		// Comparison
		bool operator==( const Matrix3& rhs ) const {
			for (uint32 i = 0; i < 9; i++) {
				if (mData[ i ] != rhs.mData[ i ]) return false;
			}
			return true;
		}
		bool operator!=( const Matrix3& rhs ) const { return !(*this == rhs); }

		const float32* Data( ) const { return &mData[ 0 ]; }

	};


}