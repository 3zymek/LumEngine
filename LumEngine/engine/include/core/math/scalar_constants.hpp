#pragma once

#include "core/types.hpp"

namespace lum {

	template<cArithmetic tType = float32>
	constexpr tType Epsilon( ) { return std::numeric_limits<tType>::epsilon( ); }

	template<cArithmetic tType = float32>
	constexpr tType Pi( ) { return static_cast< tType >(3.14159265358979323846264338327950288); }

} // namespace lum