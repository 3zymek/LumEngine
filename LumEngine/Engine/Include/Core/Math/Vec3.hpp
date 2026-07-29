//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Defines a generic 3D vector type with arithmetic operations,
//          geometric utilities and common vector aliases.
//
//=============================================================================//
#pragma once

#include "Core/Types.hpp"

namespace lum {

	/* @brief Represents a generic three-dimensional vector.
	* Provides arithmetic, scalar and comparison operations.
	* @tparam tType Arithmetic component type.
	*/
	template<cArithmetic tType>
	struct TVector3 {

		/* @brief Constructs a zero-initialized vector. */
		constexpr TVector3( ) : mX( tType( 0 ) ), mY( tType( 0 ) ), mZ( tType( 0 ) ) { }

		/* @brief Constructs a vector from individual components.
		* @param x X component.
		* @param y Y component.
		* @param z Z component.
		*/
		constexpr TVector3( tType x, tType y, tType z ) : mX( x ), mY( y ), mZ( z ) { }

		/* @brief Constructs a vector with all components initialized to the same value.
		* @param scalar Value assigned to every component.
		*/
		constexpr explicit TVector3( tType scalar ) : mX( scalar ), mY( scalar ), mZ( scalar ) { }

		/* @brief Converts the vector to another arithmetic component type.
		* @tparam tOther Destination component type.
		*/
		template<cArithmetic tOther>
		explicit operator TVector3<tOther>( ) const {
			return {
				static_cast< tOther >(mX),
				static_cast< tOther >(mY),
				static_cast< tOther >(mZ)
			};
		}

		/* @brief Vector components accessible as coordinates or color channels. */
		union {
			struct { tType mX, mY, mZ; };
			struct { tType mR, mG, mB; };
		};

		// Arithmetic

		/* @brief Adds two vectors component-wise.
		* @param rhs Vector to add.
		* @return Resulting vector.
		*/
		TVector3 operator+( const TVector3& rhs ) const { return { mX + rhs.mX, mY + rhs.mY, mZ + rhs.mZ }; }

		/* @brief Subtracts two vectors component-wise.
		* @param rhs Vector to subtract.
		* @return Resulting vector.
		*/
		TVector3 operator-( const TVector3& rhs ) const { return { mX - rhs.mX, mY - rhs.mY, mZ - rhs.mZ }; }

		/* @brief Multiplies two vectors component-wise.
		* @param rhs Vector to multiply by.
		* @return Resulting vector.
		*/
		TVector3 operator*( const TVector3& rhs ) const { return { mX * rhs.mX, mY * rhs.mY, mZ * rhs.mZ }; }

		/* @brief Divides two vectors component-wise.
		* @param rhs Vector to divide by.
		* @return Resulting vector.
		*/
		TVector3 operator/( const TVector3& rhs ) const { return { mX / rhs.mX, mY / rhs.mY, mZ / rhs.mZ }; }

		// Scalar

		/* @brief Adds a scalar to every vector component.
		* @param scalar Scalar value.
		* @return Resulting vector.
		*/
		TVector3 operator+( tType scalar ) const { return { mX + scalar, mY + scalar, mZ + scalar }; }

		/* @brief Adds a scalar to every vector component in-place.
		* @param scalar Scalar value.
		* @return Reference to this vector.
		*/
		TVector3& operator+=( tType scalar ) { mX += scalar; mY += scalar; mZ += scalar; return *this; }

		/* @brief Subtracts a scalar from every vector component.
		* @param scalar Scalar value.
		* @return Resulting vector.
		*/
		TVector3 operator-( tType scalar ) const { return { mX - scalar, mY - scalar, mZ - scalar }; }

		/* @brief Subtracts a scalar from every vector component in-place.
		* @param scalar Scalar value.
		* @return Reference to this vector.
		*/
		TVector3& operator-=( tType scalar ) { mX -= scalar; mY -= scalar; mZ -= scalar; return *this; }

		/* @brief Multiplies every vector component by a scalar.
		* @param scalar Scalar multiplier.
		* @return Resulting vector.
		*/
		TVector3 operator*( tType scalar ) const { return { mX * scalar, mY * scalar, mZ * scalar }; }

		/* @brief Multiplies every vector component by a scalar in-place.
		* @param scalar Scalar multiplier.
		* @return Reference to this vector.
		*/
		TVector3& operator*=( tType scalar ) { mX *= scalar; mY *= scalar; mZ *= scalar; return *this; }

		/* @brief Divides every vector component by a scalar.
		* @param scalar Scalar divisor.
		* @return Resulting vector.
		*/
		TVector3 operator/( tType scalar ) const {
			tType inv = tType( 1 ) / scalar;
			return { mX * inv, mY * inv, mZ * inv };
		}

		/* @brief Divides every vector component by a scalar in-place.
		* @param scalar Scalar divisor.
		* @return Reference to this vector.
		*/
		TVector3& operator/=( tType scalar ) {
			tType inv = tType( 1 ) / scalar;
			mX *= inv;
			mY *= inv;
			mZ *= inv;
			return *this;
		}

		// Assignment

		/* @brief Adds another vector to this vector.
		* @param rhs Vector to add.
		* @return Reference to this vector.
		*/
		TVector3& operator+=( const TVector3& rhs ) { mX += rhs.mX; mY += rhs.mY; mZ += rhs.mZ; return *this; }

		/* @brief Subtracts another vector from this vector.
		* @param rhs Vector to subtract.
		* @return Reference to this vector.
		*/
		TVector3& operator-=( const TVector3& rhs ) { mX -= rhs.mX; mY -= rhs.mY; mZ -= rhs.mZ; return *this; }

		// Unary

		/* @brief Negates every vector component.
		* @return Negated vector.
		*/
		TVector3 operator-( ) const { return { -mX, -mY, -mZ }; }

		// Comparison

		/* @brief Compares two vectors for equality.
		* @param rhs Vector to compare against.
		* @return True if all components are equal.
		*/
		bool operator==( const TVector3& rhs ) const { return mX == rhs.mX && mY == rhs.mY && mZ == rhs.mZ; }

		/* @brief Compares two vectors for inequality.
		* @param rhs Vector to compare against.
		* @return True if at least one component differs.
		*/
		bool operator!=( const TVector3& rhs ) const { return !(*this == rhs); }

		/* @brief Returns a pointer to the underlying vector data.
		* @return Pointer to the first vector component.
		*/
		const tType* Data( ) const { return &mX; }
		tType* Data( ) { return &mX; }

	};

	/* @brief Multiplies a vector by a scalar.
	* @tparam tType Arithmetic component type.
	* @param scalar Scalar multiplier.
	* @param v Vector to scale.
	* @return Scaled vector.
	*/
	template<cArithmetic tType>
	TVector3<tType> operator*( tType scalar, const TVector3<tType>& v ) { return v * scalar; }


	//-----------------------------------------
	// Other operations
	//-----------------------------------------

	/* @brief Returns the length of a vector.
	* @tparam tType Arithmetic component type.
	* @param v Vector to evaluate.
	* @return Vector length.
	*/
	template<cArithmetic tType>
	float32 Length( const TVector3<tType>& v ) { return sqrt( v.mX * v.mX + v.mY * v.mY + v.mZ * v.mZ ); }

	/* @brief Returns the squared length of a vector.
	* @tparam tType Arithmetic component type.
	* @param v Vector to evaluate.
	* @return Squared vector length.
	*/
	template<cArithmetic tType>
	constexpr float32 LengthSq( const TVector3<tType>& v ) { return v.mX * v.mX + v.mY * v.mY + v.mZ * v.mZ; }

	/* @brief Returns the distance between two vectors.
	* @tparam tType Arithmetic component type.
	* @param a First vector.
	* @param b Second vector.
	* @return Distance between the vectors.
	*/
	template<cArithmetic tType>
	float32 Distance( const TVector3<tType>& a, const TVector3<tType>& b ) { return Length( b - a ); }

	/* @brief Returns a normalized vector.
	* @tparam tType Arithmetic component type.
	* @param v Vector to normalize.
	* @return Normalized vector.
	*/
	template<cArithmetic tType>
	TVector3<tType> Normalize( const TVector3<tType>& v ) {
		float32 invLen = 1.0f / Length( v );
		return { v.mX * invLen, v.mY * invLen, v.mZ * invLen };
	}

	/* @brief Computes the dot product of two vectors.
	* @tparam tType Arithmetic component type.
	* @param v1 First vector.
	* @param v2 Second vector.
	* @return Dot product.
	*/
	template<cArithmetic tType>
	float32 Dot( const TVector3<tType>& v1, const TVector3<tType>& v2 ) {
		return v1.mX * v2.mX + v1.mY * v2.mY + v1.mZ * v2.mZ;
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
			v1.mY * v2.mZ - v1.mZ * v2.mY,
			v1.mZ * v2.mX - v1.mX * v2.mZ,
			v1.mX * v2.mY - v1.mY * v2.mX
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
			std::clamp( v.mX, min, max ),
			std::clamp( v.mY, min, max ),
			std::clamp( v.mZ, min, max )
		};
	}

	/* @brief Returns the component-wise absolute value of a vector.
	* @tparam tType Arithmetic component type.
	* @param v Vector to evaluate.
	* @return Vector with absolute component values.
	*/
	template<cArithmetic tType>
	TVector3<tType> Abs( const TVector3<tType>& v ) {
		return { std::abs( v.mX ), std::abs( v.mY ), std::abs( v.mZ ) };
	}

	/* @brief 3D vector using 32-bit floating-point components. */
	using Vector3 = TVector3<float32>;

	/* @brief 3D vector using 32-bit unsigned integer components. */
	using Vector3u = TVector3<uint32>;

} // namespace lum