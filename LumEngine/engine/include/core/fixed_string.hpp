#pragma once
#include "core/core_common.hpp"
namespace lum {

	template<usize L>
	struct fstring {

		template<usize new_L>
		explicit fstring(const char (&src)[new_L]) noexcept {
			static_assert(new_L <= L && "string is too big");
			size = new_L - 1;
			std::memcpy(buff, src, new_L);
		}
		explicit fstring() noexcept {
			buff[0] = '\0';
		}

		template<usize new_L>
		fstring& operator=(const char(&src)[new_L]) noexcept {
			static_assert(new_L <= L && "string is too big");
			size = new_L - 1;
			std::memcpy(buff, src, new_L);
			return *this;
		}

		template<usize new_L>
		bool operator==(const char(&src)[new_L]) const noexcept {
			if(hash(src) == hash(buff))
				return true;
			return false;
		}

		template<usize new_L>
		bool operator==(const fstring& oth) const noexcept {
			if (hash(oth.buff) == hash(buff))
				return true;
			return false;
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
	struct fstring_view {

	};

}
