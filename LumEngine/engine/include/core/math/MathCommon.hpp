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