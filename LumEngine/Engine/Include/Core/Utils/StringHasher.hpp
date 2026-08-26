//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Hash strings based on FNV-1a algorithm.
//
//=============================================================================//
#pragma once

#include "Core/Types.hpp"

namespace lum {

	// Hashed string representation using FNV-1a (64-bit).
	// Use cstd::StringHasher::Hash() to produce values of this type.
	using HashedString = uint64;

}

namespace lum::cstd {

	// Stateless string hasher using the FNV-1a (64-bit) algorithm.
	// Supports both compile-time hashing of string literals and
	// runtime hashing of StringView.
	class StringHasher {
	public:

		// Compile-time hash for string literals.
		// Template parameter L is deduced from the array size (includes null terminator).
		template<usize tL>
		inline constexpr static HashedString Hash( const char( &str )[ tL ] ) noexcept {

			HashedString h = sk_Offset;

			for (usize i = 0; i < tL - 1; i++) {
				h ^= static_cast<uint8>( str[ i ] );
				h *= sk_Prime;
			}

			return h;

		}

		// Runtime hash for StringView.
		inline constexpr static HashedString Hash( StringView str ) noexcept {

			HashedString h = sk_Offset;

			for (auto s : str) {
				h ^= static_cast<uint8>(s);
				h *= sk_Prime;
			}

			return h;

		}

	private:

		static constexpr uint64 sk_Offset = 14695981039346656037ULL; // FNV offset basis
		static constexpr uint64 sk_Prime = 1099511628211ULL;        // FNV prime

		StringHasher( ) = delete;

	};

} // namespace lum::cstd