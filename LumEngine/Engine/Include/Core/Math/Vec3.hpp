//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Defines a generic 3D vector type with arithmetic operations,
//          geometric utilities and common vector aliases.
//
//=============================================================================//
#pragma once

#include <algorithm>
#include <cmath>

#include "Core/CoreTypes.hpp"

namespace lum {

	template<cArithmetic tType>
	struct TVector2;

	template<cArithmetic tType>
	struct TVector4;

	/* @brief Represents a generic three-dimensional vector.
	* Provides arithmetic, scalar and comparison operations.
	* @tparam tType Arithmetic component type.
	*/
	template<cArithmetic tType>
	struct TVector3 {

		/* @brief Constructs a zero-initialized vector. */
		constexpr TVector3( ) : m_X( tType( 0 ) ), m_Y( tType( 0 ) ), m_Z( tType( 0 ) ) {}

		/* @brief Constructs a vector from individual components.
		* @param x X component.
		* @param y Y component.
		* @param z Z component.
		*/
		constexpr TVector3( tType x, tType y, tType z ) : m_X( x ), m_Y( y ), m_Z( z ) {}

		/* @brief Constructs a vector with all components initialized to the same value.
		* @param scalar Value assigned to every component.
		*/
		constexpr explicit TVector3( tType scalar ) : m_X( scalar ), m_Y( scalar ), m_Z( scalar ) {}

		// Cross-vector constructors

		/* @brief Constructs a 3D vector from a 2D vector and a Z component.
		* @tparam tOther Source component type.
		* @param v Source 2D vector.
		* @param z Z component.
		*/
		template<cArithmetic tOther>
		constexpr TVector3( const TVector2<tOther>& v, tOther z );

		/* @brief Constructs a 3D vector from a 4D vector, discarding W.
		* @tparam tOther Source component type.
		* @param v Source 4D vector.
		*/
		template<cArithmetic tOther>
		constexpr TVector3( const TVector4<tOther>& v );

		// Conversion operators

		/* @brief Converts the vector to another arithmetic component type.
		* @tparam tOther Destination component type.
		*/
		template<cArithmetic tOther>
		explicit operator TVector3<tOther>( ) const {
			return {
				static_cast<tOther>(m_X),
				static_cast<tOther>(m_Y),
				static_cast<tOther>(m_Z)
			};
		}

		/* @brief Converts the vector to a 2D vector, discarding Z.
		* @tparam tOther Destination component type.
		*/
		template<cArithmetic tOther>
		explicit operator TVector2<tOther>( ) const;

		/* @brief Converts the vector to a 4D vector, setting W to zero.
		* @tparam tOther Destination component type.
		*/
		template<cArithmetic tOther>
		explicit operator TVector4<tOther>( ) const;

		/* @brief Vector components accessible as coordinates or color channels. */
		union {
			struct { tType m_X, m_Y, m_Z; };
			struct { tType m_R, m_G, m_B; };
		};

		// Arithmetic

		TVector3 operator+( const TVector3& rhs ) const {
			return { m_X + rhs.m_X, m_Y + rhs.m_Y, m_Z + rhs.m_Z };
		}

		TVector3 operator-( const TVector3& rhs ) const {
			return { m_X - rhs.m_X, m_Y - rhs.m_Y, m_Z - rhs.m_Z };
		}

		TVector3 operator*( const TVector3& rhs ) const {
			return { m_X * rhs.m_X, m_Y * rhs.m_Y, m_Z * rhs.m_Z };
		}

		TVector3 operator/( const TVector3& rhs ) const {
			return { m_X / rhs.m_X, m_Y / rhs.m_Y, m_Z / rhs.m_Z };
		}

		// Scalar

		TVector3 operator+( tType scalar ) const {
			return { m_X + scalar, m_Y + scalar, m_Z + scalar };
		}

		TVector3& operator+=( tType scalar ) {
			m_X += scalar; m_Y += scalar; m_Z += scalar;
			return *this;
		}

		TVector3 operator-( tType scalar ) const {
			return { m_X - scalar, m_Y - scalar, m_Z - scalar };
		}

		TVector3& operator-=( tType scalar ) {
			m_X -= scalar; m_Y -= scalar; m_Z -= scalar;
			return *this;
		}

		TVector3 operator*( tType scalar ) const {
			return { m_X * scalar, m_Y * scalar, m_Z * scalar };
		}

		TVector3& operator*=( tType scalar ) {
			m_X *= scalar; m_Y *= scalar; m_Z *= scalar;
			return *this;
		}

		TVector3 operator/( tType scalar ) const {
			tType inv = tType( 1 ) / scalar;
			return { m_X * inv, m_Y * inv, m_Z * inv };
		}

		TVector3& operator/=( tType scalar ) {
			tType inv = tType( 1 ) / scalar;
			m_X *= inv;
			m_Y *= inv;
			m_Z *= inv;
			return *this;
		}

		// Assignment

		TVector3& operator+=( const TVector3& rhs ) {
			m_X += rhs.m_X; m_Y += rhs.m_Y; m_Z += rhs.m_Z;
			return *this;
		}

		TVector3& operator-=( const TVector3& rhs ) {
			m_X -= rhs.m_X; m_Y -= rhs.m_Y; m_Z -= rhs.m_Z;
			return *this;
		}

		// Unary

		TVector3 operator-( ) const {
			return { -m_X, -m_Y, -m_Z };
		}

		// Comparison

		bool operator==( const TVector3& rhs ) const {
			return m_X == rhs.m_X && m_Y == rhs.m_Y && m_Z == rhs.m_Z;
		}

		bool operator!=( const TVector3& rhs ) const {
			return !(*this == rhs);
		}

		/* @brief Returns a pointer to the underlying vector data. */
		const tType* Data( ) const {
			return &m_X;
		}

		tType* Data( ) {
			return &m_X;
		}
	};

	template<cArithmetic tType>
	TVector3<tType> operator*( tType scalar, const TVector3<tType>& v ) {
		return v * scalar;
	}

	//-----------------------------------------
	// Other operations
	//-----------------------------------------

	/* @brief Returns the length of a vector.
	* @tparam tType Arithmetic component type.
	* @param v Vector to evaluate.
	* @return Vector length.
	*/
	template<cArithmetic tType>
	float32 Length( const TVector3<tType>& v ) {
		return sqrt( v.m_X * v.m_X + v.m_Y * v.m_Y + v.m_Z * v.m_Z );
	}

	/* @brief Returns the squared length of a vector.
	* @tparam tType Arithmetic component type.
	* @param v Vector to evaluate.
	* @return Squared vector length.
	*/
	template<cArithmetic tType>
	constexpr float32 LengthSq( const TVector3<tType>& v ) {
		return v.m_X * v.m_X + v.m_Y * v.m_Y + v.m_Z * v.m_Z;
	}

	/* @brief Returns the distance between two vectors.
	* @tparam tType Arithmetic component type.
	* @param a First vector.
	* @param b Second vector.
	* @return Distance between the vectors.
	*/
	template<cArithmetic tType>
	float32 Distance( const TVector3<tType>& a, const TVector3<tType>& b ) {
		return Length( b - a );
	}

	/* @brief Returns a normalized vector.
	* @tparam tType Arithmetic component type.
	* @param v Vector to normalize.
	* @return Normalized vector.
	*/
	template<cArithmetic tType>
	TVector3<tType> Normalize( const TVector3<tType>& v ) {
		float32 invLen = 1.0f / Length( v );
		return { v.m_X * invLen, v.m_Y * invLen, v.m_Z * invLen };
	}

	/* @brief Computes the dot product of two vectors.
	* @tparam tType Arithmetic component type.
	* @param v1 First vector.
	* @param v2 Second vector.
	* @return Dot product.
	*/
	template<cArithmetic tType>
	float32 Dot( const TVector3<tType>& v1, const TVector3<tType>& v2 ) {
		return v1.m_X * v2.m_X + v1.m_Y * v2.m_Y + v1.m_Z * v2.m_Z;
	}

	/* @brief Computes the cross product of two vectors.
	* @tparam tType Arithmetic component type.
	* @param v1 First vector.
	* @param v2 Second vector.
	* @return Cross product vector.
	*/
	template<cArithmetic tType>
	constexpr TVector3<tType> Cross( const TVector3<tType>& v1, const TVector3<tType>& v2 ) {
		return {
			v1.m_Y * v2.m_Z - v1.m_Z * v2.m_Y,
			v1.m_Z * v2.m_X - v1.m_X * v2.m_Z,
			v1.m_X * v2.m_Y - v1.m_Y * v2.m_X
		};
	}

	/* @brief Clamps every vector component to the specified range.
	* @tparam tType Arithmetic component type.
	* @param v Vector to clamp.
	* @param min Minimum allowed value.
	* @param max Maximum allowed value.
	* @return Clamped vector.
	*/
	template<cArithmetic tType>
	constexpr TVector3<tType> Clamp( const TVector3<tType>& v, tType min, tType max ) {
		return {
			Clamp( v.m_X, min, max ),
			Clamp( v.m_Y, min, max ),
			Clamp( v.m_Z, min, max )
		};
	}

	/* @brief Returns the component-wise absolute value of a vector.
	* @tparam tType Arithmetic component type.
	* @param v Vector to evaluate.
	* @return Vector with absolute component values.
	*/
	template<cArithmetic tType>
	TVector3<tType> Abs( const TVector3<tType>& v ) {
		return { std::abs( v.m_X ), std::abs( v.m_Y ), std::abs( v.m_Z ) };
	}

	/* @brief 3D vector using 32-bit floating-point components. */
	using Vector3 = TVector3<float32>;

	/* @brief 3D vector using 32-bit unsigned integer components. */
	using Vector3u = TVector3<uint32>;

} // namespace lum