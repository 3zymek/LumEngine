#pragma once

#include "core/types.hpp"
#include "core/math/scalar_constants.hpp"
#include "core/math/vec2.hpp"
#include "core/math/vec3.hpp"
#include "core/math/vec4.hpp"
#include "core/math/mat3.hpp"
#include "core/math/mat4.hpp"
#include "core/math/quaternion.hpp"
#include "core/math/matrix_operations.hpp"

namespace lum {

	template<cArithmetic tType = float32>
	constexpr tType Abs( tType val ) { return std::abs( val ); }

	template<cArithmetic tType = float32>
	constexpr tType Radians( tType degrees ) { return degrees * (Pi<tType>( ) / tType( 180 )); }

	template<cArithmetic tType = float32>
	constexpr tType Degrees( tType radians ) { return radians * (tType( 180 ) / Pi<tType>( )); }

	template<cArithmetic tType>
	constexpr bool NearlyEqual( tType a, tType b, tType eps = Epsilon<tType>( ) ) {
		return std::abs( a - b ) < eps;
	}

}