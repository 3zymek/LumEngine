//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Hash strings based on FNV-1a algorythm
//
//=============================================================================//
#pragma once

#include "core/core_pch.hpp"

namespace lum::cstd {

	class StringHasher {
	public:
		
		template<size_t L>
		inline constexpr static uint64 Hash(const char (&str)[L]) noexcept {

			uint64 h = FNV1A_ENTRY;

			for (usize i = 0; i < L; i++) {
				h ^= static_cast<uint8>(str[i]);
				h *= FNV1A_PRIME;
			}

			return h;

		}

		inline static uint64 Hash(StringView str) noexcept {

			uint64 h = FNV1A_ENTRY;

			for (auto s : str) {
				h ^= static_cast<uint8>(s);
				h *= FNV1A_PRIME;
			}

			return h;

		}

	private:

		static constexpr uint64 FNV1A_ENTRY = 14695981039346656037ULL;
		static constexpr uint64 FNV1A_PRIME = 1099511628211ULL;

		StringHasher() = delete;

	};

}
