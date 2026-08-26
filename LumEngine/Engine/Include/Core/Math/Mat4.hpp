//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Defines a lightweight 4x4 matrix type with arithmetic,
//          scalar operations, comparison operators and raw data access.
//
//=============================================================================//
#pragma once

#include "Core/Types.hpp"

namespace lum {

	/* @brief Represents a 4x4 matrix stored in column-major order.
	* Provides arithmetic, scalar and comparison operations.
	*/
	struct LUM_API Matrix4 {

		/* @brief Constructs a zero-initialized matrix. */
		constexpr Matrix4( ) { }

		/* @brief Constructs a diagonal matrix initialized with the given scalar.
		* @param scalar Value assigned to all diagonal elements.
		*/
		constexpr explicit Matrix4( float32 scalar ) {
			m_Data[ 0 ] = scalar;
			m_Data[ 5 ] = scalar;
			m_Data[ 10 ] = scalar;
			m_Data[ 15 ] = scalar;
		}

		/* @brief Raw matrix data stored in column-major order. */
		float32 m_Data[ 16 ]{};

		// Arithmetic

		/* @brief Adds two matrices element-wise.
		* @param rhs Matrix to add.
		* @return Resulting summed matrix.
		*/
		Matrix4 operator+( const Matrix4& rhs ) const {
			Matrix4 result;
			for (uint32 i = 0; i < 16; i++)
				result.m_Data[ i ] = m_Data[ i ] + rhs.m_Data[ i ];
			return result;
		}

		/* @brief Subtracts two matrices element-wise.
		* @param rhs Matrix to subtract.
		* @return Resulting difference matrix.
		*/
		Matrix4 operator-( const Matrix4& rhs ) const {
			Matrix4 result;
			for (uint32 i = 0; i < 16; i++)
				result.m_Data[ i ] = m_Data[ i ] - rhs.m_Data[ i ];
			return result;
		}

		/* @brief Multiplies two matrices.
		* @param rhs Matrix to multiply by.
		* @return Resulting product matrix.
		*/
		Matrix4 operator*( const Matrix4& rhs ) const {
			Matrix4 result;
			for (uint32 col = 0; col < 4; col++)
				for (uint32 row = 0; row < 4; row++)
					for (uint32 k = 0; k < 4; k++)
						result.m_Data[ col * 4 + row ] += m_Data[ k * 4 + row ] * rhs.m_Data[ col * 4 + k ];
			return result;
		}

		/* @brief Divides two matrices element-wise.
		* @param rhs Matrix to divide by.
		* @return Resulting quotient matrix.
		*/
		Matrix4 operator/( const Matrix4& rhs ) const {
			Matrix4 result;
			for (uint32 i = 0; i < 16; i++)
				result.m_Data[ i ] = m_Data[ i ] / rhs.m_Data[ i ];
			return result;
		}

		// Scalar

		/* @brief Multiplies every matrix element by a scalar.
		* @tparam tType Arithmetic scalar type.
		* @param scalar Scalar multiplier.
		* @return Scaled matrix.
		*/
		template<cArithmetic tType>
		Matrix4 operator*( tType scalar ) {
			Matrix4 result;
			for (uint32 i = 0; i < 16; i++)
				result.m_Data[ i ] = m_Data[ i ] * scalar;
			return result;
		}

		/* @brief Multiplies every matrix element by a scalar in-place.
		* @tparam tType Arithmetic scalar type.
		* @param scalar Scalar multiplier.
		* @return Reference to this matrix.
		*/
		template<cArithmetic tType>
		Matrix4& operator*=( tType scalar ) {
			for (uint32 i = 0; i < 16; i++)
				m_Data[ i ] *= scalar;
			return *this;
		}

		/* @brief Divides every matrix element by a scalar.
		* @tparam tType Arithmetic scalar type.
		* @param scalar Scalar divisor.
		* @return Scaled matrix.
		*/
		template<cArithmetic tType>
		Matrix4 operator/( tType scalar ) {
			Matrix4 result;
			tType inv = 1.0 / scalar;
			for (uint32 i = 0; i < 16; i++)
				result.m_Data[ i ] = m_Data[ i ] * inv;
			return result;
		}

		/* @brief Divides every matrix element by a scalar in-place.
		* @tparam tType Arithmetic scalar type.
		* @param scalar Scalar divisor.
		* @return Reference to this matrix.
		*/
		template<cArithmetic tType>
		Matrix4& operator/=( tType scalar ) {
			tType inv = 1.0 / scalar;
			for (uint32 i = 0; i < 16; i++)
				m_Data[ i ] *= inv;
			return *this;
		}

		// Assignment

		/* @brief Adds another matrix to this matrix element-wise.
		* @param rhs Matrix to add.
		* @return Reference to this matrix.
		*/
		Matrix4& operator+=( const Matrix4& rhs ) {
			for (uint32 i = 0; i < 16; i++)
				m_Data[ i ] += rhs.m_Data[ i ];
			return *this;
		}

		/* @brief Subtracts another matrix from this matrix element-wise.
		* @param rhs Matrix to subtract.
		* @return Reference to this matrix.
		*/
		Matrix4& operator-=( const Matrix4& rhs ) {
			for (uint32 i = 0; i < 16; i++)
				m_Data[ i ] -= rhs.m_Data[ i ];
			return *this;
		}

		/* @brief Multiplies this matrix by another matrix.
		* @param rhs Matrix to multiply by.
		* @return Reference to this matrix.
		*/
		Matrix4& operator*=( const Matrix4& rhs ) {
			*this = *this * rhs;
			return *this;
		}

		/* @brief Divides this matrix element-wise by another matrix.
		* @param rhs Matrix to divide by.
		* @return Reference to this matrix.
		*/
		Matrix4& operator/=( const Matrix4& rhs ) {
			for (uint32 i = 0; i < 16; i++)
				m_Data[ i ] /= rhs.m_Data[ i ];
			return *this;
		}

		// Unary

		/* @brief Negates every matrix element.
		* @return Negated matrix.
		*/
		Matrix4 operator-( ) const {
			Matrix4 result;
			for (uint32 i = 0; i < 16; i++)
				result.m_Data[ i ] = -m_Data[ i ];
			return result;
		}

		// Comparison

		/* @brief Compares two matrices for equality.
		* @param rhs Matrix to compare against.
		* @return True if all elements are equal.
		*/
		bool operator==( const Matrix4& rhs ) const {
			for (uint32 i = 0; i < 16; i++) {
				if (m_Data[ i ] != rhs.m_Data[ i ]) return false;
			}
			return true;
		}

		/* @brief Compares two matrices for inequality.
		* @param rhs Matrix to compare against.
		* @return True if at least one element differs.
		*/
		bool operator!=( const Matrix4& rhs ) const { return !(*this == rhs); }

		/* @brief Returns a pointer to the underlying matrix data.
		* @return Pointer to the first matrix element.
		*/
		const float32* Data( ) const { return &m_Data[ 0 ]; }

		/* @brief Returns a pointer to the first element of the specified column.
		* @param col Column index.
		* @return Pointer to the first element of the column.
		*/
		float32* operator[]( uint32 col ) { return &m_Data[ col * 4 ]; }

		/* @brief Returns a read-only pointer to the first element of the specified column.
		* @param col Column index.
		* @return Const pointer to the first element of the column.
		*/
		const float32* operator[]( uint32 col ) const { return &m_Data[ col * 4 ]; }

	};

} // namespace lum