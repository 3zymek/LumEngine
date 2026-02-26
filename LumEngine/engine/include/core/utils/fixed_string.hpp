#pragma once
#include "core/core_common.hpp"
namespace lum {

	template<usize tL>
	struct FixedString {

		template<usize tNewL>
		explicit FixedString(const char (&src)[tNewL]) noexcept {
			LUM_SASSERT(tNewL <= tL && "string is too big");
			size = tNewL - 1;
			std::memcpy(buff, src, tNewL);
		}
		explicit FixedString() noexcept {
			buff[0] = '\0';
		}

		template<usize tNewL>
		FixedString& operator=(const char(&src)[tNewL]) noexcept {
			usize len = tNewL;
			if (len > tL) {
				LUM_LOG_WARN("String too big, truncating: %s", src);
				len = tL;
			}
			size = len;
			std::memcpy(buff, src, len);
			buff[len] = '\0';
			return *this;
		}

		FixedString& operator=(const char* src) noexcept {
			usize len = std::strlen(src);
			if (len > tL) {
				LUM_LOG_WARN("String too big, truncating: %s", src);
				len = tL;
			}
			size = len;
			std::memcpy(buff, src, len);
			buff[len] = '\0';
			return *this;
		}

		template<usize tNewL>
		bool operator==(const char(&src)[tNewL]) const noexcept {
			if(HashStr(src) == HashStr(buff))
				return true;
			return false;
		}

		template<usize tNewL>
		bool operator==(const FixedString& oth) const noexcept {
			if (HashStr(oth.buff) == HashStr(buff))
				return true;
			return false;
		}

		constexpr void Clear() noexcept {
			size = 0;
			buff[0] = '\0';
		}

		LUM_FORCEINLINE
		constexpr usize Length() noexcept { return size; }

		LUM_FORCEINLINE
		constexpr usize MaxSize() noexcept { return tL; }

		LUM_FORCEINLINE
		constexpr const char* Data() noexcept { return buff; }

	private:

		char	buff[tL]{};
		usize	size = 0;

	};

	// TO IMPLEMENT:
	struct FStringView {

	};

}
