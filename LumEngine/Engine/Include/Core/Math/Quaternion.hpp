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
			: mX( x ), mY( y ), mZ( z ), mW( w ) { }

		/* @brief Constructs a quaternion with all components initialized to the same value.
		* @param scalar Value assigned to every component.
		*/
		constexpr explicit Quaternion( float32 scalar )
			: mX( scalar ), mY( scalar ), mZ( scalar ), mW( scalar ) { }

		/* @brief Quaternion components. */
		float32 mX{}, mY{}, mZ{}, mW{};

		// Arithmetic

		/* @brief Adds two quaternions component-wise.
		* @param rhs Quaternion to add.
		* @return Resulting quaternion.
		*/
		Quaternion operator+( const Quaternion& rhs ) const { return { mX + rhs.mX, mY + rhs.mY, mZ + rhs.mZ, mW + rhs.mW }; }

		/* @brief Subtracts two quaternions component-wise.
		* @param rhs Quaternion to subtract.
		* @return Resulting quaternion.
		*/
		Quaternion operator-( const Quaternion& rhs ) const { return { mX - rhs.mX, mY - rhs.mY, mZ - rhs.mZ, mW - rhs.mW }; }

		/* @brief Multiplies two quaternions.
		* @param rhs Quaternion to multiply by.
		* @return Resulting quaternion.
		*/
		Quaternion operator*( const Quaternion& rhs ) const {
			return {
				mW * rhs.mX + mX * rhs.mW + mY * rhs.mZ - mZ * rhs.mY,
				mW * rhs.mY - mX * rhs.mZ + mY * rhs.mW + mZ * rhs.mX,
				mW * rhs.mZ + mX * rhs.mY - mY * rhs.mX + mZ * rhs.mW,
				mW * rhs.mW - mX * rhs.mX - mY * rhs.mY - mZ * rhs.mZ
			};
		}

		/* @brief Divides two quaternions component-wise.
		* @param rhs Quaternion to divide by.
		* @return Resulting quaternion.
		*/
		Quaternion operator/( const Quaternion& rhs ) const { return { mX / rhs.mX, mY / rhs.mY, mZ / rhs.mZ, mW / rhs.mW }; }

		// Scalar

		/* @brief Multiplies every quaternion component by a scalar.
		* @tparam tType Arithmetic scalar type.
		* @param scalar Scalar multiplier.
		* @return Scaled quaternion.
		*/
		template<cArithmetic tType>
		Quaternion operator*( tType scalar ) const { return { mX * scalar, mY * scalar, mZ * scalar, mW * scalar }; }

		/* @brief Multiplies every quaternion component by a scalar in-place.
		* @tparam tType Arithmetic scalar type.
		* @param scalar Scalar multiplier.
		* @return Reference to this quaternion.
		*/
		template<cArithmetic tType>
		Quaternion& operator*=( tType scalar ) { mX *= scalar; mY *= scalar; mZ *= scalar; mW *= scalar; return *this; }

		/* @brief Divides every quaternion component by a scalar.
		* @tparam tType Arithmetic scalar type.
		* @param scalar Scalar divisor.
		* @return Scaled quaternion.
		*/
		template<cArithmetic tType>
		Quaternion operator/( tType scalar ) const {
			tType inv = tType( 1 ) / scalar;
			return { mX * inv, mY * inv, mZ * inv, mW * inv };
		}

		/* @brief Divides every quaternion component by a scalar in-place.
		* @tparam tType Arithmetic scalar type.
		* @param scalar Scalar divisor.
		* @return Reference to this quaternion.
		*/
		template<cArithmetic tType>
		Quaternion& operator/=( tType scalar ) {
			tType inv = tType( 1 ) / scalar;
			mX *= inv; mY *= inv; mZ *= inv; mW *= inv;
			return *this;
		}

		// Assignment

		/* @brief Adds another quaternion to this quaternion.
		* @param rhs Quaternion to add.
		* @return Reference to this quaternion.
		*/
		Quaternion& operator+=( const Quaternion& rhs ) { mX += rhs.mX; mY += rhs.mY; mZ += rhs.mZ; mW += rhs.mW; return *this; }

		/* @brief Subtracts another quaternion from this quaternion.
		* @param rhs Quaternion to subtract.
		* @return Reference to this quaternion.
		*/
		Quaternion& operator-=( const Quaternion& rhs ) { mX -= rhs.mX; mY -= rhs.mY; mZ -= rhs.mZ; mW -= rhs.mW; return *this; }

		// Unary

		/* @brief Negates every quaternion component.
		* @return Negated quaternion.
		*/
		Quaternion operator-( ) const { return { -mX, -mY, -mZ, -mW }; }

		// Comparison

		/* @brief Compares two quaternions for equality.
		* @param rhs Quaternion to compare against.
		* @return True if all components are equal.
		*/
		bool operator==( const Quaternion& rhs ) const { return mX == rhs.mX && mY == rhs.mY && mZ == rhs.mZ && mW == rhs.mW; }

		/* @brief Compares two quaternions for inequality.
		* @param rhs Quaternion to compare against.
		* @return True if at least one component differs.
		*/
		bool operator!=( const Quaternion& rhs ) const { return !(*this == rhs); }

		/* @brief Returns a pointer to the underlying quaternion data.
		* @return Pointer to the first quaternion component.
		*/
		const float32* Data( ) const { return &mX; }

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
	inline float32 Length( const Quaternion& q ) { return sqrt( q.mX * q.mX + q.mY * q.mY + q.mZ * q.mZ + q.mW * q.mW ); }

	/* @brief Returns the squared length of a quaternion.
	* @param q Quaternion to evaluate.
	* @return Squared quaternion length.
	*/
	inline float32 LengthSq( const Quaternion& q ) { return q.mX * q.mX + q.mY * q.mY + q.mZ * q.mZ + q.mW * q.mW; }

	/* @brief Returns a normalized quaternion.
	* @param q Quaternion to normalize.
	* @return Normalized quaternion.
	*/
	inline Quaternion Normalize( const Quaternion& q ) {
		float32 invLen = 1.0f / Length( q );
		return { q.mX * invLen, q.mY * invLen, q.mZ * invLen, q.mW * invLen };
	}

	/* @brief Computes the dot product of two quaternions.
	* @param q1 First quaternion.
	* @param q2 Second quaternion.
	* @return Dot product.
	*/
	inline float32 Dot( const Quaternion& q1, const Quaternion& q2 ) {
		return q1.mX * q2.mX + q1.mY * q2.mY + q1.mZ * q2.mZ + q1.mW * q2.mW;
	}

	/* @brief Returns the component-wise absolute value of a quaternion.
	* @param q Quaternion to evaluate.
	* @return Quaternion with absolute component values.
	*/
	inline Quaternion Abs( const Quaternion& q ) {
		return { std::abs( q.mX ), std::abs( q.mY ), std::abs( q.mZ ), std::abs( q.mW ) };
	}

	/* @brief Returns the conjugate of a quaternion.
	* @param q Quaternion to conjugate.
	* @return Conjugated quaternion.
	*/
	inline Quaternion Conjugate( const Quaternion& q ) {
		return { -q.mX, -q.mY, -q.mZ, q.mW };
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
		return FromEuler( v.mY, v.mX, v.mZ );
	}

} // namespace lum