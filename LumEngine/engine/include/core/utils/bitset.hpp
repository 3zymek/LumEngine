#pragma once

#include "core/core_common.hpp"

namespace lum {

	template<uint32 tSize>
	struct Bitset {

		uint64 mBits[tSize / 64 + 1]{};

		void Set( uint32 bit ) {
			mBits[bit / 64] |= (1ULL << (bit % 64));
		}
		void UnSet(uint32 bit) {
			mBits[bit / 64] &= ~(1ULL << (bit % 64));
		}
		bool Has(uint32 bit) {
			return (mBits[bit / 64] & (1ULL << (bit % 64))) != 0;
		}
		void Clear() {
			std::memset(mBits, 0, sizeof(mBits));
		}

	};

}