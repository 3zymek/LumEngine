#pragma once
#include "core/core_common.hpp"
namespace lum {

	template<usize L>
	struct f_string {

		template<usize new_L>
		explicit f_string(const char (&src)[new_L]) noexcept {
			static_assert(new_L <= L && "string is too big");
			size = new_L - 1;
			std::memcpy(buff, src, new_L);
		}
		explicit f_string() noexcept {
			buff[0] = '\0';
		}

		template<usize new_L>
		f_string& operator=(const char(&src)[new_L]) noexcept {
			static_assert(new_L <= L && "string is too big");
			size = new_L - 1;
			std::memcpy(buff, src, new_L);
			return *this;
		}

		template<usize new_L>
		bool operator==(const char(&src)[new_L]) noexcept {
			if (size != new_L - 1) return false;
			for (usize i = 0; i < new_L; i++) {
				if (buff[i] != src[i]) return false;
			}
			return true;
		}

		constexpr void clear() noexcept {
			size = 0;
			buff[0] = '\0';
		}

		LUM_FORCEINLINE
		constexpr usize length() noexcept { return size; }

		LUM_FORCEINLINE
		constexpr usize max_size() noexcept { return L; }

		LUM_FORCEINLINE
		constexpr const char* data() noexcept { return buff; }

	private:

		char	buff[L]{};
		usize	size = 0;

	};

	// TO IMPLEMENT:
	struct f_stringview {

	};

}
