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
		constexpr TVector4( ) : mX( tType( 0 ) ), mY( tType( 0 ) ), mZ( tType( 0 ) ), mW( tType( 0 ) ) { }

		/* @brief Constructs a vector from individual components.
		* @param x X component.
		* @param y Y component.
		* @param z Z component.
		* @param w W component.
		*/
		constexpr TVector4( tType x, tType y, tType z, tType w )
			: mX( x ), mY( y ), mZ( z ), mW( w ) { }

		/* @brief Constructs a vector with all components initialized to the same value.
		* @param scalar Value assigned to every component.
		*/
		constexpr explicit TVector4( tType scalar )
			: mX( scalar ), mY( scalar ), mZ( scalar ), mW( scalar ) { }

		/* @brief Constructs a 4D vector from a 3D vector and an additional component.
		* @tparam tOther Source component type.
		* @param v Source 3D vector.
		* @param w W component.
		*/
		template<cArithmetic tOther>
		constexpr TVector4( const TVector3<tOther>& v, tOther w )
			: mX( static_cast< tType >(v.mX) ),
			mY( static_cast< tType >(v.mY) ),
			mZ( static_cast< tType >(v.mZ) ),
			mW( static_cast< tType >(w) ) { }

		/* @brief Converts the vector to another arithmetic component type.
		* @tparam tOther Destination component type.
		*/
		template<cArithmetic tOther>
		explicit operator TVector4<tOther>( ) const {
			return {
				static_cast< tOther >(mX),
				static_cast< tOther >(mY),
				static_cast< tOther >(mZ),
				static_cast< tOther >(mW)
			};
		}

		/* @brief Converts the vector to a 3D vector.
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

		/* @brief Converts the vector to a 2D vector.
		* @tparam tOther Destination component type.
		*/
		template<cArithmetic tOther>
		explicit operator TVector2<tOther>( ) const {
			return {
				static_cast< tOther >(mX),
				static_cast< tOther >(mY)
			};
		}

		/* @brief Vector components accessible as coordinates or color channels. */
		union {
			struct { tType mX, mY, mZ, mW; };
			struct { tType mR, mG, mB, mA; };
		};

		// Arithmetic

		/* @brief Adds two vectors component-wise.
		* @param rhs Vector to add.
		* @return Resulting vector.
		*/
		TVector4 operator+( const TVector4& rhs ) const { return { mX + rhs.mX, mY + rhs.mY, mZ + rhs.mZ, mW + rhs.mW }; }

		/* @brief Subtracts two vectors component-wise.
		* @param rhs Vector to subtract.
		* @return Resulting vector.
		*/
		TVector4 operator-( const TVector4& rhs ) const { return { mX - rhs.mX, mY - rhs.mY, mZ - rhs.mZ, mW - rhs.mW }; }

		/* @brief Multiplies two vectors component-wise.
		* @param rhs Vector to multiply by.
		* @return Resulting vector.
		*/
		TVector4 operator*( const TVector4& rhs ) const { return { mX * rhs.mX, mY * rhs.mY, mZ * rhs.mZ, mW * rhs.mW }; }

		/* @brief Divides two vectors component-wise.
		* @param rhs Vector to divide by.
		* @return Resulting vector.
		*/
		TVector4 operator/( const TVector4& rhs ) const { return { mX / rhs.mX, mY / rhs.mY, mZ / rhs.mZ, mW / rhs.mW }; }

		// Scalar

		/* @brief Adds a scalar to every vector component.
		* @param scalar Scalar value.
		* @return Resulting vector.
		*/
		TVector4 operator+( tType scalar ) const { return { mX + scalar, mY + scalar, mZ + scalar, mW + scalar }; }

		/* @brief Adds a scalar to every vector component in-place.
		* @param scalar Scalar value.
		* @return Reference to this vector.
		*/
		TVector4& operator+=( tType scalar ) { mX += scalar; mY += scalar; mZ += scalar; mW += scalar; return *this; }

		/* @brief Subtracts a scalar from every vector component.
		* @param scalar Scalar value.
		* @return Resulting vector.
		*/
		TVector4 operator-( tType scalar ) const { return { mX - scalar, mY - scalar, mZ - scalar, mW - scalar }; }

		/* @brief Subtracts a scalar from every vector component in-place.
		* @param scalar Scalar value.
		* @return Reference to this vector.
		*/
		TVector4& operator-=( tType scalar ) { mX -= scalar; mY -= scalar; mZ -= scalar; mW -= scalar; return *this; }

		/* @brief Multiplies every vector component by a scalar.
		* @param scalar Scalar multiplier.
		* @return Resulting vector.
		*/
		TVector4 operator*( tType scalar ) const { return { mX * scalar, mY * scalar, mZ * scalar, mW * scalar }; }

		/* @brief Multiplies every vector component by a scalar in-place.
		* @param scalar Scalar multiplier.
		* @return Reference to this vector.
		*/
		TVector4& operator*=( tType scalar ) { mX *= scalar; mY *= scalar; mZ *= scalar; mW *= scalar; return *this; }

		/* @brief Divides every vector component by a scalar.
		* @param scalar Scalar divisor.
		* @return Resulting vector.
		*/
		TVector4 operator/( tType scalar ) const {
			tType inv = tType( 1 ) / scalar;
			return { mX * inv, mY * inv, mZ * inv, mW * inv };
		}

		/* @brief Divides every vector component by a scalar in-place.
		* @param scalar Scalar divisor.
		* @return Reference to this vector.
		*/
		TVector4& operator/=( tType scalar ) {
			tType inv = tType( 1 ) / scalar;
			mX *= inv; mY *= inv; mZ *= inv; mW *= inv;
			return *this;
		}

		// Assignment

		/* @brief Adds another vector to this vector.
		* @param rhs Vector to add.
		* @return Reference to this vector.
		*/
		TVector4& operator+=( const TVector4& rhs ) { mX += rhs.mX; mY += rhs.mY; mZ += rhs.mZ; mW += rhs.mW; return *this; }

		/* @brief Subtracts another vector from this vector.
		* @param rhs Vector to subtract.
		* @return Reference to this vector.
		*/
		TVector4& operator-=( const TVector4& rhs ) { mX -= rhs.mX; mY -= rhs.mY; mZ -= rhs.mZ; mW -= rhs.mW; return *this; }

		// Unary

		/* @brief Negates every vector component.
		* @return Negated vector.
		*/
		TVector4 operator-( ) const { return { -mX, -mY, -mZ, -mW }; }

		// Comparison

		/* @brief Compares two vectors for equality.
		* @param rhs Vector to compare against.
		* @return True if all components are equal.
		*/
		bool operator==( const TVector4& rhs ) const { return mX == rhs.mX && mY == rhs.mY && mZ == rhs.mZ && mW == rhs.mW; }

		/* @brief Compares two vectors for inequality.
		* @param rhs Vector to compare against.
		* @return True if at least one component differs.
		*/
		bool operator!=( const TVector4& rhs ) const { return !(*this == rhs); }

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
	constexpr float32 Length( const TVector4<tType>& v ) { return sqrt( v.mX * v.mX + v.mY * v.mY + v.mZ * v.mZ + v.mW * v.mW ); }

	/* @brief Returns the squared length of a vector.
	* @tparam tType Arithmetic component type.
	* @param v Vector to evaluate.
	* @return Squared vector length.
	*/
	template<cArithmetic tType>
	constexpr float32 LengthSq( const TVector4<tType>& v ) { return v.mX * v.mX + v.mY * v.mY + v.mZ * v.mZ + v.mW * v.mW; }

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
		return { v.mX * invLen, v.mY * invLen, v.mZ * invLen, v.mW * invLen };
	}

	/* @brief Computes the dot product of two vectors.
	* @tparam tType Arithmetic component type.
	* @param v1 First vector.
	* @param v2 Second vector.
	* @return Dot product.
	*/
	template<cArithmetic tType>
	constexpr float32 Dot( const TVector4<tType>& v1, const TVector4<tType>& v2 ) {
		return v1.mX * v2.mX + v1.mY * v2.mY + v1.mZ * v2.mZ + v1.mW * v2.mW;
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
			std::clamp( v.mX, min, max ),
			std::clamp( v.mY, min, max ),
			std::clamp( v.mZ, min, max ),
			std::clamp( v.mW, min, max )
		};
	}

	/* @brief Returns the component-wise absolute value of a vector.
	* @tparam tType Arithmetic component type.
	* @param v Vector to evaluate.
	* @return Vector with absolute component values.
	*/
	template<cArithmetic tType>
	constexpr TVector4<tType> Abs( const TVector4<tType>& v ) {
		return { std::abs( v.mX ), std::abs( v.mY ), std::abs( v.mZ ), std::abs( v.mW ) };
	}

	/* @brief 4D vector using 32-bit floating-point components. */
	using Vector4 = TVector4<float32>;

	/* @brief 4D vector using 32-bit unsigned integer components. */
	using Vector4u = TVector4<uint32>;

} // namespace lum