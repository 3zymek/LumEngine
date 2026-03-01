//========= Copyright (C) 2026 3zymek, MIT License ============//
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

	using ucharptr = unsigned char*;  /* @brief Pointer to unsigned byte data. */
	using ccharptr = const char*;     /* @brief Pointer to immutable C string. */
	using charptr = char*;           /* @brief Pointer to mutable C string. */

	using usize = size_t;     /* @brief Unsigned size type for counts and indices. */
	using isize = ptrdiff_t;  /* @brief Signed size type for pointer differences. */

	using byte = char;			/* @brief Single raw byte. */
	using bitfield = uint32_t;  /* @brief 32-bit field for packed boolean flags. */

	using vptr = void*;			/* @brief Pointer to untyped mutable data. */
	using cvptr = const void*;  /* @brief Pointer to untyped immutable data. */

	using String = std::string;				/* @brief Owning heap-allocated string. */
	using StringView = std::string_view;	/* @brief Non-owning view into a string. */

} // namespace lum