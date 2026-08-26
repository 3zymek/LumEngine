//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Defines a fixed-size bitset type with compile-time storage size
//          and basic bit manipulation operations.
//
//=============================================================================//
#pragma once

#include "Core/Types.hpp"

namespace lum {

	/* @brief Fixed-size bitset stored as an array of 64-bit unsigned integers.
	* Provides bit manipulation operations for compile-time defined bit counts.
	* @tparam tSize Number of bits stored in the bitset.
	*/
	template<uint32 tSize>
	struct Bitset {

		/* @brief Internal storage containing packed bits. */
		uint64 m_Bits[ (tSize + 63) / 64 ]{};

		/* @brief Sets a bit at the specified index.
		* @param bit Index of the bit to set.
		*/
		void Set( uint32 bit ) {
			LUM_ASSERT( bit < tSize, "%ul out of range of %ul Bitset", bit, tSize );
			m_Bits[ bit / 64 ] |= (1ULL << (bit % 64));
		}

		/* @brief Clears a bit at the specified index.
		* @param bit Index of the bit to clear.
		*/
		void Reset( uint32 bit ) {
			LUM_ASSERT( bit < tSize, "%ul out of range of %ul Bitset", bit, tSize );
			m_Bits[ bit / 64 ] &= ~(1ULL << (bit % 64));
		}

		/* @brief Checks whether a bit is set.
		* @param bit Index of the bit to check.
		* @return True if the bit is set, otherwise false.
		*/
		bool Has( uint32 bit ) const {
			LUM_ASSERT( bit < tSize, "%ul out of range of %ul Bitset", bit, tSize );
			return (m_Bits[ bit / 64 ] & (1ULL << (bit % 64))) != 0;
		}

		/* @brief Clears all bits in the bitset. */
		void Clear( ) {
			std::memset( m_Bits, 0, sizeof( m_Bits ) );
		}

	};

} // namespace lum