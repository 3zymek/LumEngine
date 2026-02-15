#pragma once
#include "core/core_common.hpp"
namespace lum {

	template<usize L>
	struct FString {

		template<usize new_L>
		explicit FString(const char (&src)[new_L]) noexcept {
			static_assert(new_L <= L && "string is too big");
			size = new_L - 1;
			std::memcpy(buff, src, new_L);
		}
		explicit FString() noexcept {
			buff[0] = '\0';
		}

		template<usize new_L>
		FString& operator=(const char(&src)[new_L]) noexcept {
			static_assert(new_L <= L && "string is too big");
			size = new_L - 1;
			std::memcpy(buff, src, new_L);
			return *this;
		}

		template<usize new_L>
		bool operator==(const char(&src)[new_L]) const noexcept {
			if(HashStr(src) == HashStr(buff))
				return true;
			return false;
		}

		template<usize new_L>
		bool operator==(const FString& oth) const noexcept {
			if (HashStr(oth.buff) == HashStr(buff))
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
	struct FStringView {

	};

}
