#pragma once
#include "core/core_pch.hpp"
namespace cstd {

	class StringHasher {
	public:
		
		inline constexpr static uint64_t Hash(std::string_view str) noexcept {

			constexpr uint64_t FNV1A_ENTRY = 14695981039346656037ULL;
			constexpr uint64_t FNV1A_PRIME = 1099511628211ULL;

			uint64_t h = FNV1A_ENTRY;

			for (auto c : str) {
				h ^= static_cast<uint8_t>(c);
				h *= FNV1A_PRIME;
			}

			return h;

		}

	private:

		StringHasher() = delete;

	};

}
