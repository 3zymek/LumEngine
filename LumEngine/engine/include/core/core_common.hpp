//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Core helpers and utilities for the Lum Engine.
//          Includes type conversion, size helpers, hashing and terminal color codes.
//
//=============================================================================//
#pragma once

#include "core/core_pch.hpp"
#include "core/core_defines.hpp"
#include "core/types.hpp"
#include "core/utils/string_hasher.hpp"

namespace lum {

	/* @brief Generates a unique numeric ID for a given type.
	* Each instantiation maintains its own atomic counter starting at tNullVal + 1.
	* @tparam tType  Type to generate IDs for.
	* @tparam tNullVal Sentinel null value; the counter starts one above this.
	*/
	template<typename tType, std::integral tIntegral = usize, tIntegral tNullVal = 0>
	struct GenerateID {

		/* @brief Returns the next available unique ID.
		* @return Atomically incremented uint32 identifier.
		*/
		static tIntegral Get( ) {
			return sID.fetch_add(1, std::memory_order_relaxed);
		}

	private:

		/* @brief Shared atomic counter for this type instantiation. */
		inline static std::atomic<tIntegral> sID = tNullVal + 1;

	};

	/* @brief Generates a unique compile-time stable ID per type.
	* Each distinct type receives a single ID assigned on first call.
	*/
	struct GenerateTypeID {

		/* @brief Returns the unique ID assigned to tType.
		* @tparam tType Type to retrieve the ID for.
		* @return Stable uint32 identifier for the given type.
		*/
		template<typename tType>
		static uint32 Get( ) {
			static uint32 id = sID.fetch_add(1, std::memory_order_relaxed);
			return id;
		}

	private:

		/* @brief Global atomic counter shared across all type ID assignments. */
		inline static std::atomic<uint32> sID = 0;

	};

	/* @brief Converts any arithmetic type to float32 at compile-time.
	* @param value The numeric value to cast.
	* @return The value casted to float32.
	*/
	template<typename tType>
		requires std::is_arithmetic_v<tType>
	inline constexpr float32 ToFloat32( tType value ) { return static_cast<float32>(value); }

	/* @brief Converts any arithmetic type to uint8 at compile-time.
	* @param value The numeric value to cast.
	* @return The value casted to uint8.
	*/
	template<typename tType>
		requires std::is_arithmetic_v<tType>
	inline constexpr uint8 ToUint8( tType value ) { return static_cast<uint8>(value); }

	/* @brief Converts any arithmetic type to uint16 at compile-time.
	* @param value The numeric value to cast.
	* @return The value casted to uint16.
	*/
	template<typename tType>
		requires std::is_arithmetic_v<tType>
	inline constexpr uint16 ToUint16( tType value ) { return static_cast<uint16>(value); }

	/* @brief Converts any arithmetic type to uint32 at compile-time.
	* @param value The numeric value to cast.
	* @return The value casted to uint32.
	*/
	template<typename tType>
		requires std::is_arithmetic_v<tType>
	inline constexpr uint32 ToUint32( tType value ) { return static_cast<uint32>(value); }

	/* @brief Converts any arithmetic type to uint64 at compile-time.
	* @param value The numeric value to cast.
	* @return The value casted to uint64.
	*/
	template<typename tType>
		requires std::is_arithmetic_v<tType>
	inline constexpr uint64 ToUint64( tType value ) { return static_cast<uint64>(value); }

	/* @brief Converts any arithmetic type to float64 at compile-time.
	* @param value The numeric value to cast.
	* @return The value casted to float64.
	*/
	template<typename tType>
		requires std::is_arithmetic_v<tType>
	inline constexpr float64 ToFloat64( tType value ) { return static_cast<float64>(value); }

	/* @brief Converts any arithmetic type to int32 at compile-time.
	* @param value The numeric value to cast.
	* @return The value casted to int32.
	*/
	template<typename tType>
		requires std::is_arithmetic_v<tType>
	inline constexpr int32 ToInteger( tType value ) { return static_cast<int32>(value); }

	/* @brief Returns the maximum representable value for an arithmetic type.
	* @tparam T Arithmetic type to query.
	* @return std::numeric_limits<T>::max().
	*/
	template<typename tType>
		requires std::is_arithmetic_v<tType>
	inline constexpr tType MaxVal( ) {
		return std::numeric_limits<tType>::max();
	}

	/* @brief Returns the total byte size of a std::vector's contents.
	* @param vector Vector to calculate byte size for.
	* @return sizeof(T) * vector.size().
	*/
	template<typename tType>
	inline constexpr usize ByteSize( const std::vector<tType>& vector ) {
		return sizeof(tType) * vector.size();
	}

	/* @brief Returns the total byte size of a raw array.
	* @param arr Reference to the raw array.
	* @return sizeof(T) * L.
	*/
	template<typename tType, usize tL>
	inline constexpr usize ByteSize( const tType(&arr)[tL] ) {
		return sizeof(tType) * tL;
	}

	/* @brief Returns the total byte size of a pointer-based array.
	* @param arr  Pointer to the data.
	* @param size Number of elements.
	* @return sizeof(T) * size.
	*/
	template<typename tType>
	inline constexpr usize ByteSize( const tType* arr, usize size ) {
		return sizeof(tType) * size;
	}

	/* @brief Returns the element count of a raw array.
	* @param arr Reference to the raw array.
	* @return Number of elements L.
	*/
	template<typename tType, usize tL>
	inline constexpr usize ArraySize( const tType(&arr)[tL] ) {
		return tL;
	}

	/* @brief Casts an enum value to its underlying integral type.
	* @param e Enum value to convert.
	* @return The underlying integer value of the enum.
	*/
	template<typename tEnum>
		requires std::is_enum_v<tEnum>
	inline constexpr std::underlying_type_t<tEnum> ToUnderlyingEnum( tEnum e ) {
		return static_cast<std::underlying_type_t<tEnum>>(e);
	}

	/* @brief Hashes a UTF-8 string literal to a 64-bit unsigned integer at compile-time.
	* @tparam L Size of the string literal including null terminator.
	* @param str String literal to hash.
	* @return 64-bit hash of the input string.
	*/
	template<usize tL>
	inline constexpr uint64 HashStr( const char(&str)[tL] ) noexcept {
		return cstd::StringHasher::Hash( str );
	}

	/* @brief Hashes a StringView to a 64-bit unsigned integer at runtime.
	* @param str String view to hash.
	* @return 64-bit hash of the input string.
	*/
	inline uint64 HashStr( StringView str ) noexcept {
		return cstd::StringHasher::Hash( str );
	}

	/* @brief Converts a String to its lowercase equivalent.
	* @param str Input string to convert.
	* @return New string with all characters lowercased.
	*/
	inline String ToLower( StringView str ) {
		String result = str.data( );
		std::transform( result.begin( ), result.end( ), result.begin( ), ::tolower );
		return result;
	}

	/* @brief Converts a String to its uppercase equivalent.
	* @param str Input string to convert.
	* @return New string with all characters upeercased.
	*/
	inline String ToUpper( StringView str ) {
		String result = str.data( );
		std::transform( result.begin( ), result.end( ), result.begin( ), ::toupper );
		return result;
	}

	/* @brief ANSI escape code constants for terminal text and background coloring.
	* Use these with standard output streams to colorize console messages.
	*/
	namespace cmdcolor {

		inline constexpr ccharptr Reset = "\033[0m";
		inline constexpr ccharptr Black = "\033[30m";
		inline constexpr ccharptr Red = "\033[31m";
		inline constexpr ccharptr Green = "\033[32m";
		inline constexpr ccharptr Yellow = "\033[33m";
		inline constexpr ccharptr Blue = "\033[34m";
		inline constexpr ccharptr Magenta = "\033[35m";
		inline constexpr ccharptr Cyan = "\033[36m";
		inline constexpr ccharptr White = "\033[37m";

		inline constexpr ccharptr LightBlack = "\033[90m";
		inline constexpr ccharptr LightRed = "\033[91m";
		inline constexpr ccharptr LightGreen = "\033[92m";
		inline constexpr ccharptr LightYellow = "\033[93m";
		inline constexpr ccharptr LightBlue = "\033[94m";
		inline constexpr ccharptr LightMagenta = "\033[95m";
		inline constexpr ccharptr LightCyan = "\033[96m";
		inline constexpr ccharptr LightWhite = "\033[97m";

		inline constexpr ccharptr BgBlack = "\033[40m";
		inline constexpr ccharptr BgRed = "\033[41m";
		inline constexpr ccharptr BgGreen = "\033[42m";
		inline constexpr ccharptr BgYellow = "\033[43m";
		inline constexpr ccharptr BgBlue = "\033[44m";
		inline constexpr ccharptr BgMagenta = "\033[45m";
		inline constexpr ccharptr BgCyan = "\033[46m";
		inline constexpr ccharptr BgWhite = "\033[47m";

		inline constexpr ccharptr BgLightBlack = "\033[100m";
		inline constexpr ccharptr BgLightRed = "\033[101m";
		inline constexpr ccharptr BgLightGreen = "\033[102m";
		inline constexpr ccharptr BgLightYellow = "\033[103m";
		inline constexpr ccharptr BgLightBlue = "\033[104m";
		inline constexpr ccharptr BgLightMagenta = "\033[105m";
		inline constexpr ccharptr BgLightCyan = "\033[106m";
		inline constexpr ccharptr BgLightWhite = "\033[107m";

		inline constexpr ccharptr FontBold = "\033[1m";
		inline constexpr ccharptr FontItalic = "\033[3m";
		inline constexpr ccharptr FontUnderline = "\033[4m";
		inline constexpr ccharptr FontStrikethrough = "\033[9m";
		inline constexpr ccharptr FontReserve = "\033[7m";

	} // namespace lum::cmdcolor

} // namespace lum