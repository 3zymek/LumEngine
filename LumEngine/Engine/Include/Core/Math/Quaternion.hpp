//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Defines a quaternion type with arithmetic operations,
//          normalization, interpolation helpers and Euler angle conversion.
//
//=============================================================================//
#pragma once

#include "Core/Types.hpp"

namespace lum {

	/* @brief Represents a quaternion used for 3D rotations.
	* Provides arithmetic, scalar and comparison operations.
	*/
	struct LUM_API Quaternion {

		/* @brief Constructs a quaternion from individual components.
		* @param x X component.
		* @param y Y component.
		* @param z Z component.
		* @param w W component.
		*/
		constexpr Quaternion( float32 x, float32 y, float32 z, float32 w )
			: m_X( x ), m_Y( y ), m_Z( z ), m_W( w ) { }

		/* @brief Constructs a quaternion with all components initialized to the same value.
		* @param scalar Value assigned to every component.
		*/
		constexpr explicit Quaternion( float32 scalar )
			: m_X( scalar ), m_Y( scalar ), m_Z( scalar ), m_W( scalar ) { }

		/* @brief Quaternion components. */
		float32 m_X{}, m_Y{}, m_Z{}, m_W{};

		// Arithmetic

		/* @brief Adds two quaternions component-wise.
		* @param rhs Quaternion to add.
		* @return Resulting quaternion.
		*/
		Quaternion operator+( const Quaternion& rhs ) const { return { m_X + rhs.m_X, m_Y + rhs.m_Y, m_Z + rhs.m_Z, m_W + rhs.m_W }; }

		/* @brief Subtracts two quaternions component-wise.
		* @param rhs Quaternion to subtract.
		* @return Resulting quaternion.
		*/
		Quaternion operator-( const Quaternion& rhs ) const { return { m_X - rhs.m_X, m_Y - rhs.m_Y, m_Z - rhs.m_Z, m_W - rhs.m_W }; }

		/* @brief Multiplies two quaternions.
		* @param rhs Quaternion to multiply by.
		* @return Resulting quaternion.
		*/
		Quaternion operator*( const Quaternion& rhs ) const {
			return {
				m_W * rhs.m_X + m_X * rhs.m_W + m_Y * rhs.m_Z - m_Z * rhs.m_Y,
				m_W * rhs.m_Y - m_X * rhs.m_Z + m_Y * rhs.m_W + m_Z * rhs.m_X,
				m_W * rhs.m_Z + m_X * rhs.m_Y - m_Y * rhs.m_X + m_Z * rhs.m_W,
				m_W * rhs.m_W - m_X * rhs.m_X - m_Y * rhs.m_Y - m_Z * rhs.m_Z
			};
		}

		/* @brief Divides two quaternions component-wise.
		* @param rhs Quaternion to divide by.
		* @return Resulting quaternion.
		*/
		Quaternion operator/( const Quaternion& rhs ) const { return { m_X / rhs.m_X, m_Y / rhs.m_Y, m_Z / rhs.m_Z, m_W / rhs.m_W }; }

		// Scalar

		/* @brief Multiplies every quaternion component by a scalar.
		* @tparam tType Arithmetic scalar type.
		* @param scalar Scalar multiplier.
		* @return Scaled quaternion.
		*/
		template<cArithmetic tType>
		Quaternion operator*( tType scalar ) const { return { m_X * scalar, m_Y * scalar, m_Z * scalar, m_W * scalar }; }

		/* @brief Multiplies every quaternion component by a scalar in-place.
		* @tparam tType Arithmetic scalar type.
		* @param scalar Scalar multiplier.
		* @return Reference to this quaternion.
		*/
		template<cArithmetic tType>
		Quaternion& operator*=( tType scalar ) { m_X *= scalar; m_Y *= scalar; m_Z *= scalar; m_W *= scalar; return *this; }

		/* @brief Divides every quaternion component by a scalar.
		* @tparam tType Arithmetic scalar type.
		* @param scalar Scalar divisor.
		* @return Scaled quaternion.
		*/
		template<cArithmetic tType>
		Quaternion operator/( tType scalar ) const {
			tType inv = tType( 1 ) / scalar;
			return { m_X * inv, m_Y * inv, m_Z * inv, m_W * inv };
		}

		/* @brief Divides every quaternion component by a scalar in-place.
		* @tparam tType Arithmetic scalar type.
		* @param scalar Scalar divisor.
		* @return Reference to this quaternion.
		*/
		template<cArithmetic tType>
		Quaternion& operator/=( tType scalar ) {
			tType inv = tType( 1 ) / scalar;
			m_X *= inv; m_Y *= inv; m_Z *= inv; m_W *= inv;
			return *this;
		}

		// Assignment

		/* @brief Adds another quaternion to this quaternion.
		* @param rhs Quaternion to add.
		* @return Reference to this quaternion.
		*/
		Quaternion& operator+=( const Quaternion& rhs ) { m_X += rhs.m_X; m_Y += rhs.m_Y; m_Z += rhs.m_Z; m_W += rhs.m_W; return *this; }

		/* @brief Subtracts another quaternion from this quaternion.
		* @param rhs Quaternion to subtract.
		* @return Reference to this quaternion.
		*/
		Quaternion& operator-=( const Quaternion& rhs ) { m_X -= rhs.m_X; m_Y -= rhs.m_Y; m_Z -= rhs.m_Z; m_W -= rhs.m_W; return *this; }

		// Unary

		/* @brief Negates every quaternion component.
		* @return Negated quaternion.
		*/
		Quaternion operator-( ) const { return { -m_X, -m_Y, -m_Z, -m_W }; }

		// Comparison

		/* @brief Compares two quaternions for equality.
		* @param rhs Quaternion to compare against.
		* @return True if all components are equal.
		*/
		bool operator==( const Quaternion& rhs ) const { return m_X == rhs.m_X && m_Y == rhs.m_Y && m_Z == rhs.m_Z && m_W == rhs.m_W; }

		/* @brief Compares two quaternions for inequality.
		* @param rhs Quaternion to compare against.
		* @return True if at least one component differs.
		*/
		bool operator!=( const Quaternion& rhs ) const { return !(*this == rhs); }

		/* @brief Returns a pointer to the underlying quaternion data.
		* @return Pointer to the first quaternion component.
		*/
		const float32* Data( ) const { return &m_X; }

	};

	/* @brief Multiplies a quaternion by a scalar.
	* @tparam tType Arithmetic scalar type.
	* @param scalar Scalar multiplier.
	* @param q Quaternion to scale.
	* @return Scaled quaternion.
	*/
	template<cArithmetic tType>
	Quaternion operator*( tType scalar, const Quaternion& q ) { return q * scalar; }



	//-----------------------------------------
	// Other operations
	//-----------------------------------------

	/* @brief Returns the length of a quaternion.
	* @param q Quaternion to evaluate.
	* @return Quaternion length.
	*/
	inline float32 Length( const Quaternion& q ) { return sqrt( q.m_X * q.m_X + q.m_Y * q.m_Y + q.m_Z * q.m_Z + q.m_W * q.m_W ); }

	/* @brief Returns the squared length of a quaternion.
	* @param q Quaternion to evaluate.
	* @return Squared quaternion length.
	*/
	inline float32 LengthSq( const Quaternion& q ) { return q.m_X * q.m_X + q.m_Y * q.m_Y + q.m_Z * q.m_Z + q.m_W * q.m_W; }

	/* @brief Returns a normalized quaternion.
	* @param q Quaternion to normalize.
	* @return Normalized quaternion.
	*/
	inline Quaternion Normalize( const Quaternion& q ) {
		float32 invLen = 1.0f / Length( q );
		return { q.m_X * invLen, q.m_Y * invLen, q.m_Z * invLen, q.m_W * invLen };
	}

	/* @brief Computes the dot product of two quaternions.
	* @param q1 First quaternion.
	* @param q2 Second quaternion.
	* @return Dot product.
	*/
	inline float32 Dot( const Quaternion& q1, const Quaternion& q2 ) {
		return q1.m_X * q2.m_X + q1.m_Y * q2.m_Y + q1.m_Z * q2.m_Z + q1.m_W * q2.m_W;
	}

	/* @brief Returns the component-wise absolute value of a quaternion.
	* @param q Quaternion to evaluate.
	* @return Quaternion with absolute component values.
	*/
	inline Quaternion Abs( const Quaternion& q ) {
		return { std::abs( q.m_X ), std::abs( q.m_Y ), std::abs( q.m_Z ), std::abs( q.m_W ) };
	}

	/* @brief Returns the conjugate of a quaternion.
	* @param q Quaternion to conjugate.
	* @return Conjugated quaternion.
	*/
	inline Quaternion Conjugate( const Quaternion& q ) {
		return { -q.m_X, -q.m_Y, -q.m_Z, q.m_W };
	}

	/* @brief Returns the inverse of a quaternion.
	* @param q Quaternion to invert.
	* @return Inverted quaternion.
	*/
	inline Quaternion Inverse( const Quaternion& q ) {
		return Conjugate( q ) / LengthSq( q );
	}

	/* @brief Creates a quaternion from Euler angles.
	* @param pitch Rotation around the X axis in radians.
	* @param roll Rotation around the Z axis in radians.
	* @param yaw Rotation around the Y axis in radians.
	* @return Quaternion representing the specified rotation.
	*/
	inline Quaternion FromEuler( float32 pitch, float32 roll, float32 yaw ) {
		float32 cp = cos( pitch * 0.5f ), sp = sin( pitch * 0.5f );
		float32 cy = cos( yaw * 0.5f ), sy = sin( yaw * 0.5f );
		float32 cr = cos( roll * 0.5f ), sr = sin( roll * 0.5f );
		return {
			sr * cp * cy - cr * sp * sy,
			cr * sp * cy + sr * cp * sy,
			cr * cp * sy - sr * sp * cy,
			cr * cp * cy + sr * sp * sy
		};
	}

	/* @brief Creates a quaternion from Euler angles stored in a vector.
	* @param v Vector containing Euler angles in radians.
	* @return Quaternion representing the specified rotation.
	*/
	inline Quaternion FromEuler( const Vector3& v ) {
		return FromEuler( v.m_Y, v.m_X, v.m_Z );
	}

} // namespace lum