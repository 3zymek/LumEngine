//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Arithmetic type wrapper enforcing compile-time clamped range
//
//=============================================================================//

#pragma once

#include "core/core_common.hpp"

namespace lum {

	/* @brief Wraps an arithmetic value and clamps it to[ tMin, tMax ] on every write.
	*  Implicitly converts to tType, so it can be used anywhere the underlying
	*  type is expected without extra syntax.
	*/
	template<cArithmetic tType, tType tMin = MinValue<tType>( ), tType tMax = MaxValue<tType>( )>
	struct Ranged {

		tType mValue = {};

		constexpr Ranged( tType val ) : mValue( std::clamp( val, tMin, tMax ) ) { }
		constexpr Ranged& operator=( tType oth ) { mValue = std::clamp( oth, tMin, tMax ); return *this; }
		constexpr operator tType( ) const { return mValue; }

	};

} // namespace lum