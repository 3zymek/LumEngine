//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Provides common mathematical utilities, scalar conversions,
//          comparisons and includes the core math library components.
//
//=============================================================================//
#pragma once

#include "Core/Types.hpp"
#include "Core/Math/ScalarConstants.hpp"
#include "Core/Math/Vec2.hpp"
#include "Core/Math/Vec3.hpp"
#include "Core/Math/Vec4.hpp"
#include "Core/Math/Mat3.hpp"
#include "Core/Math/Mat4.hpp"
#include "Core/Math/Quaternion.hpp"
#include "Core/Math/MatrixOperations.hpp"

namespace lum {

	/* @brief Returns the absolute value of a numeric value.
	* @tparam tType Arithmetic type.
	* @param val Value to evaluate.
	* @return Absolute value of the input.
	*/
	template<cArithmetic tType = float32>
	LUM_NODISCARD constexpr tType Abs( tType val ) { return std::abs( val ); }

	/* @brief Converts an angle from degrees to radians.
	* @tparam tType Arithmetic type.
	* @param degrees Angle in degrees.
	* @return Equivalent angle in radians.
	*/
	template<cArithmetic tType = float32>
	LUM_NODISCARD constexpr tType Radians( tType degrees ) { return degrees * (Pi<tType>( ) / tType( 180 )); }

	/* @brief Converts an angle from radians to degrees.
	* @tparam tType Arithmetic type.
	* @param radians Angle in radians.
	* @return Equivalent angle in degrees.
	*/
	template<cArithmetic tType = float32>
	LUM_NODISCARD constexpr tType Degrees( tType radians ) { return radians * (tType( 180 ) / Pi<tType>( )); }

	/* @brief Compares two numeric values using an epsilon tolerance.
	* @tparam tType Arithmetic type.
	* @param a First value.
	* @param b Second value.
	* @param eps Comparison tolerance.
	* @return True if the values differ by less than the specified epsilon.
	*/
	template<cArithmetic tType>
	LUM_NODISCARD constexpr bool NearlyEqual( tType a, tType b, tType eps = Epsilon<tType>( ) ) {
		return std::abs( a - b ) < eps;
	}
	
	/* @brief Clamps a numeric value between a specified minimum and maximum boundary.
	* @tparam tType Arithmetic type.
	* @param val Value to be clamped.
	* @param min Lower boundary.
	* @param max Upper boundary.
	* @return Minimum if val < min, maximum if val > max, otherwise val.
	*/
	template<cArithmetic tType>
	LUM_NODISCARD constexpr tType Clamp( tType val, tType min, tType max ) noexcept {
		return (val < min) ? min : ((val > max) ? max : val);
	}

} // namespace lum