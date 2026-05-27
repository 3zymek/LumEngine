//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Easy management and bitflags operations for enums
// 
//=============================================================================//
#pragma once

namespace lum {

	/*!
	* @brief Type-safe bitflag wrapper for enum types.
	*
	* @brief Flags provides a convenient way to manage bitflags using enum types.
	* It supports enable/disable operations, checking if flags are set, and
	* bitwise operations through operator overloads.
	*
	* @tparam T Enum type to wrap (must satisfy Enum concept)
	*/
	template<cEnum tType>
	struct Flags {

		using UnderlyingType = std::underlying_type_t<tType>;

		UnderlyingType mFlags = 0;

		constexpr Flags( ) noexcept = default;
		constexpr Flags( tType flag ) noexcept : mFlags( static_cast<UnderlyingType>(flag) ) {}
		constexpr Flags( std::initializer_list<tType> list ) {
			for (tType f : list)
				Enable(f);
		}

		/*!
		* @brief Enables (sets) a single flag.
		*
		* Performs bitwise OR to add the flag to the current set.
		*
		* @param flags Flag to enable.
		*/
		constexpr void Enable( tType flags ) noexcept { mFlags |= static_cast<UnderlyingType>(flags); }

		/*!
		* @brief Enables (sets) multiple flags.
		*
		* Performs bitwise OR to merge the given flags into the current set.
		*		
		* @param flags Flags to enable.
		*/
		constexpr void Enable( Flags<tType> flags ) noexcept { mFlags |= flags.mFlags; }
		/*!
		* @brief Disables (clears) a single flag.
		*
		* Performs bitwise AND with negation to remove the flag.
		*
		* @param flags Flag to disable.
		*/
		constexpr void Disable( tType flags ) noexcept { mFlags &= ~static_cast<UnderlyingType>(flags); }

		/*!
		* @brief Disables (clears) multiple flags.
		*
		* Performs bitwise AND with negation to remove the given flags.
		*
		* @param flags Flags to disable.
		*/
		constexpr void Disable( Flags<tType> flags ) noexcept { mFlags &= ~flags.mFlags; }

		/*!
		* @brief Checks if no flags are set.
		*
		* @return True if the flag set is empty (all bits are zero).
		*/
		constexpr bool IsEmpty( ) const noexcept { return mFlags == 0; }

		/*!
		* @brief Checks if a specific flag is set.
		*
		* @param flag Flag to check.
		* @return True if the flag is set, false otherwise.
		*/
		constexpr bool Has( tType flag ) const noexcept { return (mFlags & static_cast<UnderlyingType>(flag)) != 0; }

		/*!
		* @brief Checks if only the allowed flags are set.
		*
		* Verifies that no flags outside the allowed set are enabled.
		*
		* @param allowed Set of allowed flags.
		* @return True if only allowed flags are set, false otherwise.
		*/
		constexpr bool HasOnly( Flags<tType> allowed ) const noexcept { return (mFlags & ~allowed.mFlags) == 0; }

		/*!
		* @brief Clears all flags.
		*
		* Resets the internal flag state to zero.
		*/
		constexpr void Clear( ) noexcept { mFlags = 0; }

		constexpr Flags& operator=( tType flag ) noexcept {
			mFlags = static_cast<UnderlyingType>(flag);
			return *this;
		}

	};

	template<cEnum tType>
	struct EnableEnumFlags : std::false_type {};

	/* @brief Enables bitwise flag operations for a given enum class.
	*
	* Specializes EnableEnumFlags for type T to mark it as a valid flag enum.
	* After using this macro, the enum supports bitwise operators
	* (|, &, ^, ~) via the EnableEnumFlags trait.
	*
	* @param T Enum class type to enable flag operations for.
	*
	* @example
	*   enum class ShaderStage { Vertex = 1, Fragment = 2, Compute = 4 };
	*   LUM_ENUM_OPERATIONS(ShaderStage);
	*   ShaderStage stages = ShaderStage::Vertex | ShaderStage::Fragment;
	*/
#	define LUM_ENABLE_ENUM_BITFLAG_OPERATIONS(tType) \
		template<> \
		struct EnableEnumFlags<tType> : std::true_type {}

	template<cEnum tType>
		requires EnableEnumFlags<tType>::value
	constexpr Flags<tType> operator|( tType a, tType b ) {
		Flags<tType> f;
		f.Enable(a);
		f.Enable(b);
		return f;
	}

	template<cEnum tType>
		requires EnableEnumFlags<tType>::value
	constexpr Flags<tType> operator|( Flags<tType> a, tType b ) {
		a.Enable(b);
		return a;
	}

	template<cEnum tType>
		requires EnableEnumFlags<tType>::value
	constexpr Flags<tType>& operator|=( Flags<tType>& a, tType b ) {
		a.Enable(b);
		return a;
	}

	
	template<cEnum tType>
		requires EnableEnumFlags<tType>::value
	constexpr Flags<tType> operator&( tType a, tType b ) {
		Flags<tType> result;
		if ((static_cast<std::underlying_type_t<tType>>(a) &
			static_cast<std::underlying_type_t<tType>>(b)) != 0) {
			result.Enable(a & b);
		}
		return result;
	}

} // namespace lum