//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Easy management and bitflags operations for enums
// 
//=============================================================================//
#pragma once

#include "core/core_common.hpp"

namespace lum {

	template<typename T>
	concept Enum = std::is_enum_v<T>;

	/*!
	* @brief Type-safe bitflag wrapper for enum types.
	*
	* @brief Flags provides a convenient way to manage bitflags using enum types.
	* It supports enable/disable operations, checking if flags are set, and
	* bitwise operations through operator overloads.
	*
	* @tparam T Enum type to wrap (must satisfy Enum concept)
	*/
	template<Enum T>
	struct Flags {

		using U = std::underlying_type_t<T>;

		U mFlags = 0;

		constexpr Flags() noexcept = default;
		constexpr Flags(T flag) noexcept : mFlags(static_cast<U>(flag)) {}

		/*!
		* @brief Enables (sets) a single flag.
		*
		* Performs bitwise OR to add the flag to the current set.
		*
		* @param flags Flag to enable.
		*/
		constexpr void Enable(T flags) noexcept {
			mFlags |= static_cast<U>(flags);
		}

		/*!
		* @brief Enables (sets) multiple flags.
		*
		* Performs bitwise OR to merge the given flags into the current set.
		*		
		* @param flags Flags to enable.
		*/
		constexpr void Enable(Flags<T> flags) noexcept {
			mFlags |= flags.mFlags;
		}
		/*!
		* @brief Disables (clears) a single flag.
		*
		* Performs bitwise AND with negation to remove the flag.
		*
		* @param flags Flag to disable.
		*/
		constexpr void Disable(T flags) noexcept {
			mFlags &= ~static_cast<U>(flags);
		}

		/*!
		* @brief Disables (clears) multiple flags.
		*
		* Performs bitwise AND with negation to remove the given flags.
		*
		* @param flags Flags to disable.
		*/
		constexpr void Disable(Flags<T> flags) noexcept {
			mFlags &= ~flags.mFlags;
		}

		/*!
		* @brief Checks if a specific flag is set.
		*
		* @param flag Flag to check.
		* @return True if the flag is set, false otherwise.
		*/
		constexpr bool Has(T flag) const noexcept {
			return (mFlags & static_cast<U>(flag)) != 0;
		}

		/*!
		* @brief Checks if only the allowed flags are set.
		*
		* Verifies that no flags outside the allowed set are enabled.
		*
		* @param allowed Set of allowed flags.
		* @return True if only allowed flags are set, false otherwise.
		*/
		constexpr bool HasOnly(Flags<T> allowed) const noexcept {
			return (mFlags & ~allowed.mFlags) == 0;
		}

		/*!
		* @brief Clears all flags.
		*
		* Resets the internal flag state to zero.
		*/
		constexpr void Clear() noexcept {
			mFlags = 0;
		}

		constexpr Flags& operator=(T flag) noexcept {
			mFlags = static_cast<U>(flag);
			return *this;
		}

	};

	template<Enum T>
	constexpr Flags<T> operator|(T a, T b) {
		return Flags<T>(a) | b;
	}
	template<Enum T>
	constexpr Flags<T> operator|(Flags<T> a, T b) {
		a.Enable(b);
		return a;
	}

	template<Enum T>
	constexpr Flags<T>& operator|=(Flags<T>& a, T b) {
		a.Enable(b);
		return a;
	}

	
	template<Enum T>
	constexpr Flags<T> operator&(T a, T b) {
		Flags<T> result;
		if ((static_cast<std::underlying_type_t<T>>(a) &
			static_cast<std::underlying_type_t<T>>(b)) != 0) {
			result.Enable(a & b);
		}
		return result;
	}

}