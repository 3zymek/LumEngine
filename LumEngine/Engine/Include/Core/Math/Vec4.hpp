//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Defines a generic 4D vector type with arithmetic operations,
//          geometric utilities and common vector aliases.
//
//=============================================================================//
#pragma once

#include "Core/Types.hpp"

namespace lum {

	/* @brief Represents a generic four-dimensional vector.
	* Provides arithmetic, scalar and comparison operations.
	* @tparam tType Arithmetic component type.
	*/
	template<cArithmetic tType>
	struct TVector4 {

		/* @brief Constructs a zero-initialized vector. */
		constexpr TVector4( ) : m_X( tType( 0 ) ), m_Y( tType( 0 ) ), m_Z( tType( 0 ) ), m_W( tType( 0 ) ) { }

		/* @brief Constructs a vector from individual components.
		* @param x X component.
		* @param y Y component.
		* @param z Z component.
		* @param w W component.
		*/
		constexpr TVector4( tType x, tType y, tType z, tType w )
			: m_X( x ), m_Y( y ), m_Z( z ), m_W( w ) { }

		/* @brief Constructs a vector with all components initialized to the same value.
		* @param scalar Value assigned to every component.
		*/
		constexpr explicit TVector4( tType scalar )
			: m_X( scalar ), m_Y( scalar ), m_Z( scalar ), m_W( scalar ) { }

		/* @brief Constructs a 4D vector from a 3D vector and an additional component.
		* @tparam tOther Source component type.
		* @param v Source 3D vector.
		* @param w W component.
		*/
		template<cArithmetic tOther>
		constexpr TVector4( const TVector3<tOther>& v, tOther w )
			: m_X( static_cast< tType >(v.m_X) ),
			m_Y( static_cast< tType >(v.m_Y) ),
			m_Z( static_cast< tType >(v.m_Z) ),
			m_W( static_cast< tType >(w) ) { }

		/* @brief Converts the vector to another arithmetic component type.
		* @tparam tOther Destination component type.
		*/
		template<cArithmetic tOther>
		explicit operator TVector4<tOther>( ) const {
			return {
				static_cast< tOther >(m_X),
				static_cast< tOther >(m_Y),
				static_cast< tOther >(m_Z),
				static_cast< tOther >(m_W)
			};
		}

		/* @brief Converts the vector to a 3D vector.
		* @tparam tOther Destination component type.
		*/
		template<cArithmetic tOther>
		explicit operator TVector3<tOther>( ) const {
			return {
				static_cast< tOther >(m_X),
				static_cast< tOther >(m_Y),
				static_cast< tOther >(m_Z)
			};
		}

		/* @brief Converts the vector to a 2D vector.
		* @tparam tOther Destination component type.
		*/
		template<cArithmetic tOther>
		explicit operator TVector2<tOther>( ) const {
			return {
				static_cast< tOther >(m_X),
				static_cast< tOther >(m_Y)
			};
		}

		/* @brief Vector components accessible as coordinates or color channels. */
		union {
			struct { tType m_X, m_Y, m_Z, m_W; };
			struct { tType m_R, m_G, m_B, m_A; };
		};

		// Arithmetic

		/* @brief Adds two vectors component-wise.
		* @param rhs Vector to add.
		* @return Resulting vector.
		*/
		TVector4 operator+( const TVector4& rhs ) const { return { m_X + rhs.m_X, m_Y + rhs.m_Y, m_Z + rhs.m_Z, m_W + rhs.m_W }; }

		/* @brief Subtracts two vectors component-wise.
		* @param rhs Vector to subtract.
		* @return Resulting vector.
		*/
		TVector4 operator-( const TVector4& rhs ) const { return { m_X - rhs.m_X, m_Y - rhs.m_Y, m_Z - rhs.m_Z, m_W - rhs.m_W }; }

		/* @brief Multiplies two vectors component-wise.
		* @param rhs Vector to multiply by.
		* @return Resulting vector.
		*/
		TVector4 operator*( const TVector4& rhs ) const { return { m_X * rhs.m_X, m_Y * rhs.m_Y, m_Z * rhs.m_Z, m_W * rhs.m_W }; }

		/* @brief Divides two vectors component-wise.
		* @param rhs Vector to divide by.
		* @return Resulting vector.
		*/
		TVector4 operator/( const TVector4& rhs ) const { return { m_X / rhs.m_X, m_Y / rhs.m_Y, m_Z / rhs.m_Z, m_W / rhs.m_W }; }

		// Scalar

		/* @brief Adds a scalar to every vector component.
		* @param scalar Scalar value.
		* @return Resulting vector.
		*/
		TVector4 operator+( tType scalar ) const { return { m_X + scalar, m_Y + scalar, m_Z + scalar, m_W + scalar }; }

		/* @brief Adds a scalar to every vector component in-place.
		* @param scalar Scalar value.
		* @return Reference to this vector.
		*/
		TVector4& operator+=( tType scalar ) { m_X += scalar; m_Y += scalar; m_Z += scalar; m_W += scalar; return *this; }

		/* @brief Subtracts a scalar from every vector component.
		* @param scalar Scalar value.
		* @return Resulting vector.
		*/
		TVector4 operator-( tType scalar ) const { return { m_X - scalar, m_Y - scalar, m_Z - scalar, m_W - scalar }; }

		/* @brief Subtracts a scalar from every vector component in-place.
		* @param scalar Scalar value.
		* @return Reference to this vector.
		*/
		TVector4& operator-=( tType scalar ) { m_X -= scalar; m_Y -= scalar; m_Z -= scalar; m_W -= scalar; return *this; }

		/* @brief Multiplies every vector component by a scalar.
		* @param scalar Scalar multiplier.
		* @return Resulting vector.
		*/
		TVector4 operator*( tType scalar ) const { return { m_X * scalar, m_Y * scalar, m_Z * scalar, m_W * scalar }; }

		/* @brief Multiplies every vector component by a scalar in-place.
		* @param scalar Scalar multiplier.
		* @return Reference to this vector.
		*/
		TVector4& operator*=( tType scalar ) { m_X *= scalar; m_Y *= scalar; m_Z *= scalar; m_W *= scalar; return *this; }

		/* @brief Divides every vector component by a scalar.
		* @param scalar Scalar divisor.
		* @return Resulting vector.
		*/
		TVector4 operator/( tType scalar ) const {
			tType inv = tType( 1 ) / scalar;
			return { m_X * inv, m_Y * inv, m_Z * inv, m_W * inv };
		}

		/* @brief Divides every vector component by a scalar in-place.
		* @param scalar Scalar divisor.
		* @return Reference to this vector.
		*/
		TVector4& operator/=( tType scalar ) {
			tType inv = tType( 1 ) / scalar;
			m_X *= inv; m_Y *= inv; m_Z *= inv; m_W *= inv;
			return *this;
		}

		// Assignment

		/* @brief Adds another vector to this vector.
		* @param rhs Vector to add.
		* @return Reference to this vector.
		*/
		TVector4& operator+=( const TVector4& rhs ) { m_X += rhs.m_X; m_Y += rhs.m_Y; m_Z += rhs.m_Z; m_W += rhs.m_W; return *this; }

		/* @brief Subtracts another vector from this vector.
		* @param rhs Vector to subtract.
		* @return Reference to this vector.
		*/
		TVector4& operator-=( const TVector4& rhs ) { m_X -= rhs.m_X; m_Y -= rhs.m_Y; m_Z -= rhs.m_Z; m_W -= rhs.m_W; return *this; }

		// Unary

		/* @brief Negates every vector component.
		* @return Negated vector.
		*/
		TVector4 operator-( ) const { return { -m_X, -m_Y, -m_Z, -m_W }; }

		// Comparison

		/* @brief Compares two vectors for equality.
		* @param rhs Vector to compare against.
		* @return True if all components are equal.
		*/
		bool operator==( const TVector4& rhs ) const { return m_X == rhs.m_X && m_Y == rhs.m_Y && m_Z == rhs.m_Z && m_W == rhs.m_W; }

		/* @brief Compares two vectors for inequality.
		* @param rhs Vector to compare against.
		* @return True if at least one component differs.
		*/
		bool operator!=( const TVector4& rhs ) const { return !(*this == rhs); }

		/* @brief Returns a pointer to the underlying vector data.
		* @return Pointer to the first vector component.
		*/
		const tType* Data( ) const { return &m_X; }
		tType* Data( ) { return &m_X; }

	};

	/* @brief Multiplies a vector by a scalar.
	* @tparam tType Arithmetic component type.
	* @param scalar Scalar multiplier.
	* @param v Vector to scale.
	* @return Scaled vector.
	*/
	template<cArithmetic tType>
	TVector4<tType> operator*( tType scalar, const TVector4<tType>& v ) { return v * scalar; }


	//-----------------------------------------
	// Other operations
	//-----------------------------------------

	/* @brief Returns the length of a vector.
	* @tparam tType Arithmetic component type.
	* @param v Vector to evaluate.
	* @return Vector length.
	*/
	template<cArithmetic tType>
	constexpr float32 Length( const TVector4<tType>& v ) { return sqrt( v.m_X * v.m_X + v.m_Y * v.m_Y + v.m_Z * v.m_Z + v.m_W * v.m_W ); }

	/* @brief Returns the squared length of a vector.
	* @tparam tType Arithmetic component type.
	* @param v Vector to evaluate.
	* @return Squared vector length.
	*/
	template<cArithmetic tType>
	constexpr float32 LengthSq( const TVector4<tType>& v ) { return v.m_X * v.m_X + v.m_Y * v.m_Y + v.m_Z * v.m_Z + v.m_W * v.m_W; }

	/* @brief Returns the distance between two vectors.
	* @tparam tType Arithmetic component type.
	* @param a First vector.
	* @param b Second vector.
	* @return Distance between the vectors.
	*/
	template<cArithmetic tType>
	constexpr float32 Distance( const TVector4<tType>& a, const TVector4<tType>& b ) { return Length( b - a ); }

	/* @brief Returns a normalized vector.
	* @tparam tType Arithmetic component type.
	* @param v Vector to normalize.
	* @return Normalized vector.
	*/
	template<cArithmetic tType>
	constexpr TVector4<tType> Normalize( const TVector4<tType>& v ) {
		float32 invLen = 1.0f / Length( v );
		return { v.m_X * invLen, v.m_Y * invLen, v.m_Z * invLen, v.m_W * invLen };
	}

	/* @brief Computes the dot product of two vectors.
	* @tparam tType Arithmetic component type.
	* @param v1 First vector.
	* @param v2 Second vector.
	* @return Dot product.
	*/
	template<cArithmetic tType>
	constexpr float32 Dot( const TVector4<tType>& v1, const TVector4<tType>& v2 ) {
		return v1.m_X * v2.m_X + v1.m_Y * v2.m_Y + v1.m_Z * v2.m_Z + v1.m_W * v2.m_W;
	}

	/* @brief Clamps every vector component to the specified range.
	* @tparam tType Arithmetic component type.
	* @param v Vector to clamp.
	* @param min Minimum allowed value.
	* @param max Maximum allowed value.
	* @return Clamped vector.
	*/
	template<cArithmetic tType>
	constexpr TVector4<tType> Clamp( const TVector4<tType>& v, tType min, tType max ) {
		return {
			std::clamp( v.m_X, min, max ),
			std::clamp( v.m_Y, min, max ),
			std::clamp( v.m_Z, min, max ),
			std::clamp( v.m_W, min, max )
		};
	}

	/* @brief Returns the component-wise absolute value of a vector.
	* @tparam tType Arithmetic component type.
	* @param v Vector to evaluate.
	* @return Vector with absolute component values.
	*/
	template<cArithmetic tType>
	constexpr TVector4<tType> Abs( const TVector4<tType>& v ) {
		return { std::abs( v.m_X ), std::abs( v.m_Y ), std::abs( v.m_Z ), std::abs( v.m_W ) };
	}

	/* @brief 4D vector using 32-bit floating-point components. */
	using Vector4 = TVector4<float32>;

	/* @brief 4D vector using 32-bit unsigned integer components. */
	using Vector4u = TVector4<uint32>;

} // namespace lum