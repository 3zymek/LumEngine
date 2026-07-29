//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Provides commonly used mathematical scalar constants.
//
//=============================================================================//
#pragma once

#include "Core/Types.hpp"

namespace lum {

	/* @brief Returns the machine epsilon for the specified arithmetic type.
	* @tparam tType Arithmetic type.
	* @return Machine epsilon value.
	*/
	template<cArithmetic tType = float32>
	constexpr tType Epsilon( ) { return std::numeric_limits<tType>::epsilon( ); }

	/* @brief Returns the mathematical constant π.
	* @tparam tType Arithmetic type.
	* @return Value of π represented as the requested type.
	*/
	template<cArithmetic tType = float32>
	constexpr tType Pi( ) { return static_cast< tType >(3.14159265358979323846264338327950288); }

} // namespace lum