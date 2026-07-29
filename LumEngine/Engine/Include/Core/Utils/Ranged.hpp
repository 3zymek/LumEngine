//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Defines a compile-time ranged arithmetic wrapper that
//          automatically clamps assigned values to a fixed interval.
//
//=============================================================================//
#pragma once

#include "Core/CoreCommon.hpp"

namespace lum {

	/* @brief Arithmetic value wrapper with compile-time minimum and maximum bounds.
	*
	* Automatically clamps assigned values to the range [tMin, tMax].
	* Implicitly converts to the underlying arithmetic type, allowing it to be
	* used wherever the wrapped type is expected.
	*
	* @tparam tType Underlying arithmetic type.
	* @tparam tMin Minimum allowed value.
	* @tparam tMax Maximum allowed value.
	*/
	template<cArithmetic tType, tType tMin = MinValue<tType>( ), tType tMax = MaxValue<tType>( )>
	struct Ranged {

		/* @brief Stored clamped value. */
		tType mValue{};

		/* @brief Constructs a ranged value.
		* @param val Initial value, clamped to the valid range.
		*/
		constexpr Ranged( tType val )
			: mValue( std::clamp( val, tMin, tMax ) ) { }

		/* @brief Assigns a new value.
		* @param oth Value to assign.
		* @return Reference to this object.
		*/
		constexpr Ranged& operator=( tType oth ) {
			mValue = std::clamp( oth, tMin, tMax );
			return *this;
		}

		/* @brief Implicitly converts to the underlying arithmetic type.
		* @return Stored clamped value.
		*/
		constexpr operator tType( ) const {
			return mValue;
		}

	};

} // namespace lum