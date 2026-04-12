#pragma once
#include "core/types.hpp"

namespace lum {

	// Fixed-size bitset backed by a uint64 array.
	// tSize = number of bits to store.
	template<uint32 tSize>
	struct Bitset {

		uint64 mBits[ tSize / 64 + 1 ]{};

		// Sets bit at given index.
		void Set( uint32 bit ) {
			mBits[ bit / 64 ] |= (1ULL << (bit % 64));
		}

		// Clears bit at given index.
		void UnSet( uint32 bit ) {
			mBits[ bit / 64 ] &= ~(1ULL << (bit % 64));
		}

		// Returns true if bit at given index is set.
		bool Has( uint32 bit ) const {
			return (mBits[ bit / 64 ] & (1ULL << (bit % 64))) != 0;
		}

		// Clears all bits.
		void Clear( ) {
			std::memset( mBits, 0, sizeof( mBits ) );
		}

	};

} // namespace lum