//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Types used in LumEngine
//
//=============================================================================//
#pragma once

/* @brief Root namespace for all LumEngine types, systems and subsystems. */
namespace lum {

	using float32 = float;
	using float64 = double;

	using int64 = int64_t;
	using int32 = int32_t;
	using int16 = int16_t;
	using int8 = int8_t;

	using uint64 = uint64_t;
	using uint32 = uint32_t;
	using uint16 = uint16_t;
	using uint8 = uint8_t;

	using char8 = char8_t;
	using char16 = char16_t;
	using char32 = char32_t;

	using usize = size_t;     /* @brief Unsigned size type for counts and indices. */
	using isize = ptrdiff_t;  /* @brief Signed size type for pointer differences. */

	using byte = uint8;			/* @brief Single raw byte. */
	using bitfield = uint32_t;  /* @brief 32-bit field for packed boolean flags. */

	using String = std::string;				/* @brief Owning heap-allocated string. */
	using StringView = std::string_view;	/* @brief Non-owning view into a string. */



	//------------------------------------------------
	// Concepts
	//------------------------------------------------

	/* @brief Constrains tType to arithmetic types (integral or floating point). */
	template<typename tType>
	concept cArithmetic = std::is_arithmetic_v<tType>;

	/* @brief Constrains tType to types representable as a C string (char or string-like objects). */
	template<typename tType>
	concept cStringable = std::same_as<tType, char> || requires(tType t) {
		{ t.data( ) } -> std::convertible_to<const char*>;
		{ t.size( ) } -> std::convertible_to<usize>;
	};

	/* @brief Constrains tType to enum types. */
	template<typename tType>
	concept cEnum = std::is_enum_v<tType>;

	/* @brief Constrains tType to floating point types (float, double, long double). */
	template<typename tType>
	concept cFloating = std::is_floating_point_v<tType>;


} // namespace lum