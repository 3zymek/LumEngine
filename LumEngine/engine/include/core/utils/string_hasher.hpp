#pragma once
#include "core/core_pch.hpp"
namespace cstd {

	class StringHasher {
	public:
		
		template<size_t L>
		inline constexpr static uint64_t Hash(const char (&str)[L]) noexcept {

			uint64_t h = FNV1A_ENTRY;

			for (size_t i = 0; i < L; i++) {
				h ^= static_cast<uint8_t>(str[i]);
				h *= FNV1A_PRIME;
			}

			return h;

		}

		inline static uint64_t Hash(std::string_view str) noexcept {

			uint64_t h = FNV1A_ENTRY;

			for (auto s : str) {
				h ^= static_cast<uint8_t>(s);
				h *= FNV1A_PRIME;
			}

			return h;

		}

	private:

		static constexpr uint64_t FNV1A_ENTRY = 14695981039346656037ULL;
		static constexpr uint64_t FNV1A_PRIME = 1099511628211ULL;

		StringHasher() = delete;

	};

}
