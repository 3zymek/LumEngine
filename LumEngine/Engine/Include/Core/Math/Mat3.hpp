//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Defines a lightweight 3x3 matrix type with arithmetic,
//          scalar operations, comparison operators and raw data access.
//
//=============================================================================//
#pragma once

#include "Core/Types.hpp"

namespace lum {

	/* @brief Represents a 3x3 matrix stored in column-major order.
	* Provides arithmetic operations, scalar operations and comparison utilities.
	*/
	struct LUM_API Matrix3 {

		/* @brief Constructs a zero-initialized matrix. */
		constexpr Matrix3( ) { }

		/* @brief Constructs a diagonal matrix initialized with the given scalar.
		* @param scalar Value assigned to all diagonal elements.
		*/
		constexpr explicit Matrix3( float32 scalar ) {
			mData[ 0 ] = scalar;
			mData[ 4 ] = scalar;
			mData[ 8 ] = scalar;
		}

		/* @brief Raw matrix data stored in column-major order. */
		float32 mData[ 9 ]{};

		// Arithmetic

		/* @brief Adds two matrices element-wise.
		* @param rhs Matrix to add.
		* @return Resulting summed matrix.
		*/
		Matrix3 operator+( const Matrix3& rhs ) const {
			Matrix3 result;
			for (uint32 i = 0; i < 9; i++)
				result.mData[ i ] = mData[ i ] + rhs.mData[ i ];
			return result;
		}

		/* @brief Subtracts two matrices element-wise.
		* @param rhs Matrix to subtract.
		* @return Resulting difference matrix.
		*/
		Matrix3 operator-( const Matrix3& rhs ) const {
			Matrix3 result;
			for (uint32 i = 0; i < 9; i++)
				result.mData[ i ] = mData[ i ] - rhs.mData[ i ];
			return result;
		}

		/* @brief Multiplies two matrices.
		* @param rhs Matrix to multiply by.
		* @return Resulting product matrix.
		*/
		Matrix3 operator*( const Matrix3& rhs ) const {
			Matrix3 result;
			for (uint32 col = 0; col < 3; col++)
				for (uint32 row = 0; row < 3; row++)
					for (uint32 k = 0; k < 3; k++)
						result.mData[ col * 3 + row ] += mData[ k * 3 + row ] * rhs.mData[ col * 3 + k ];
			return result;
		}

		/* @brief Divides two matrices element-wise.
		* @param rhs Matrix to divide by.
		* @return Resulting quotient matrix.
		*/
		Matrix3 operator/( const Matrix3& rhs ) const {
			Matrix3 result;
			for (uint32 i = 0; i < 9; i++)
				result.mData[ i ] = mData[ i ] / rhs.mData[ i ];
			return result;
		}

		// Scalar

		/* @brief Multiplies every matrix element by a scalar.
		* @tparam tType Arithmetic scalar type.
		* @param scalar Scalar multiplier.
		* @return Scaled matrix.
		*/
		template<cArithmetic tType>
		Matrix3 operator*( tType scalar ) {
			Matrix3 result;
			for (uint32 i = 0; i < 9; i++)
				result.mData[ i ] = mData[ i ] * scalar;
			return result;
		}

		/* @brief Multiplies every matrix element by a scalar in-place.
		* @tparam tType Arithmetic scalar type.
		* @param scalar Scalar multiplier.
		* @return Reference to this matrix.
		*/
		template<cArithmetic tType>
		Matrix3& operator*=( tType scalar ) {
			for (uint32 i = 0; i < 9; i++)
				mData[ i ] *= scalar;
			return *this;
		}

		/* @brief Divides every matrix element by a scalar.
		* @tparam tType Arithmetic scalar type.
		* @param scalar Scalar divisor.
		* @return Scaled matrix.
		*/
		template<cArithmetic tType>
		Matrix3 operator/( tType scalar ) {
			Matrix3 result;
			tType inv = 1.0 / scalar;
			for (uint32 i = 0; i < 9; i++)
				result.mData[ i ] = mData[ i ] * inv;
			return result;
		}

		/* @brief Divides every matrix element by a scalar in-place.
		* @tparam tType Arithmetic scalar type.
		* @param scalar Scalar divisor.
		* @return Reference to this matrix.
		*/
		template<cArithmetic tType>
		Matrix3& operator/=( tType scalar ) {
			tType inv = 1.0 / scalar;
			for (uint32 i = 0; i < 9; i++)
				mData[ i ] *= inv;
			return *this;
		}

		// Assignment

		/* @brief Adds another matrix to this matrix element-wise.
		* @param rhs Matrix to add.
		* @return Reference to this matrix.
		*/
		Matrix3& operator+=( const Matrix3& rhs ) {
			for (uint32 i = 0; i < 9; i++)
				mData[ i ] += rhs.mData[ i ];
			return *this;
		}

		/* @brief Subtracts another matrix from this matrix element-wise.
		* @param rhs Matrix to subtract.
		* @return Reference to this matrix.
		*/
		Matrix3& operator-=( const Matrix3& rhs ) {
			for (uint32 i = 0; i < 9; i++)
				mData[ i ] -= rhs.mData[ i ];
			return *this;
		}

		/* @brief Multiplies this matrix by another matrix.
		* @param rhs Matrix to multiply by.
		* @return Reference to this matrix.
		*/
		Matrix3& operator*=( const Matrix3& rhs ) {
			*this = *this * rhs;
			return *this;
		}

		/* @brief Divides this matrix element-wise by another matrix.
		* @param rhs Matrix to divide by.
		* @return Reference to this matrix.
		*/
		Matrix3& operator/=( const Matrix3& rhs ) {
			for (uint32 i = 0; i < 9; i++)
				mData[ i ] /= rhs.mData[ i ];
			return *this;
		}

		// Unary

		/* @brief Negates every matrix element.
		* @return Negated matrix.
		*/
		Matrix3 operator-( ) const {
			Matrix3 result;
			for (uint32 i = 0; i < 9; i++)
				result.mData[ i ] = -mData[ i ];
			return result;
		}

		// Comparison

		/* @brief Compares two matrices for equality.
		* @param rhs Matrix to compare against.
		* @return True if all elements are equal.
		*/
		bool operator==( const Matrix3& rhs ) const {
			for (uint32 i = 0; i < 9; i++) {
				if (mData[ i ] != rhs.mData[ i ]) return false;
			}
			return true;
		}

		/* @brief Compares two matrices for inequality.
		* @param rhs Matrix to compare against.
		* @return True if at least one element differs.
		*/
		bool operator!=( const Matrix3& rhs ) const { return !(*this == rhs); }

		/* @brief Returns a pointer to the underlying matrix data.
		* @return Pointer to the first matrix element.
		*/
		const float32* Data( ) const { return &mData[ 0 ]; }

	};

} // namespace lum