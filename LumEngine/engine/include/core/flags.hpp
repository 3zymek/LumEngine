#pragma once
#include "core/core_common.hpp"
namespace lum {

	template<typename T>
	concept Enum = std::is_enum_v<T>;

	template<Enum T>
	struct Flags {

		using U = std::underlying_type_t<T>;

		U mFlags = 0;

		constexpr Flags() noexcept = default;
		constexpr explicit Flags(T flag) noexcept : mFlags(static_cast<U>(flag)) {}

		constexpr void enable(T flag) noexcept {
			mFlags |= static_cast<U>(flag);
		}
		constexpr void disable(T flag) noexcept {
			mFlags &= ~static_cast<U>(flag);
		}
		constexpr bool has(T flag) const noexcept {
			return (mFlags & static_cast<U>(flag)) != 0;
		}
		constexpr void clear() noexcept {
			mFlags = 0;
		}

	};

	template<Enum T>
	constexpr Flags<T> operator|(T a, T b) {
		return Flags<T>(a) | b;
	}
	template<Enum T>
	constexpr Flags<T> operator|(Flags<T> a, T b) {
		a.enable(b);
		return a;
	}

	template<Enum T>
	constexpr Flags<T>& operator|=(Flags<T>& a, T b) {
		a.enable(b);
		return a;
	}

	
	template<Enum T>
	constexpr Flags<T> operator&(T a, T b) {
		Flags<T> result;
		if ((static_cast<std::underlying_type_t<T>>(a) &
			static_cast<std::underlying_type_t<T>>(b)) != 0) {
			result.enable(a & b);
		}
		return result;
	}

}