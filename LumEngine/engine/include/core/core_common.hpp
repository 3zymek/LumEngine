//========= Copyright (C) 2025-present 3zymek, MIT License ============//
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
#include "core/utils/logger.hpp"
#include "core/utils/lum_assert.hpp"
#include "core/math/math_common.hpp"

namespace lum {

	/* @brief Generates a unique numeric ID for a given type.
	* Each instantiation maintains its own atomic counter starting at tNullVal + 1.
	* @tparam tType  Type to generate IDs for.
	* @tparam tNullVal Sentinel null value; the counter starts one above this.
	*/
	template<typename tType, std::integral tIntegral = usize, tIntegral tNullVal = 0>
	struct UniqueID {

		/* @brief Returns the next available unique ID.
		* @return Atomically incremented uint32 identifier.
		*/
		static tIntegral Get( ) {
			return sID.fetch_add( 1, std::memory_order_relaxed );
		}

	private:

		/* @brief Shared atomic counter for this type instantiation. */
		inline static std::atomic<tIntegral> sID = tNullVal + 1;

	};

	/* @brief Generates a unique compile-time stable ID per type.
	* Each distinct type receives a single ID assigned on first call.
	*/
	struct RuntimeTypeID {

		/* @brief Returns the unique ID assigned to tType.
		* @tparam tType Type to retrieve the ID for.
		* @return Stable uint32 identifier for the given type.
		*/
		template<typename tType>
		static uint32 Get( ) {
			static uint32 id = sID.fetch_add( 1, std::memory_order_relaxed );
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
	template<cArithmetic tType>
	inline constexpr float32 ToFloat32( tType value ) { return static_cast< float32 >(value); }

	/* @brief Converts any arithmetic type to uint8 at compile-time.
	* @param value The numeric value to cast.
	* @return The value casted to uint8.
	*/
	template<cArithmetic tType>
	inline constexpr uint8 ToUint8( tType value ) { return static_cast< uint8 >(value); }

	/* @brief Converts any arithmetic type to int8 at compile-time.
	* @param value The numeric value to cast.
	* @return The value casted to int8.
	*/
	template<cArithmetic tType>
	inline constexpr int8 ToInt8( tType value ) { return static_cast< int8 >(value); }

	/* @brief Converts any arithmetic type to uint16 at compile-time.
	* @param value The numeric value to cast.
	* @return The value casted to uint16.
	*/
	template<cArithmetic tType>
	inline constexpr uint16 ToUint16( tType value ) { return static_cast< uint16 >(value); }

	/* @brief Converts any arithmetic type to int16 at compile-time.
	* @param value The numeric value to cast.
	* @return The value casted to int16.
	*/
	template<cArithmetic tType>
	inline constexpr int16 ToInt16( tType value ) { return static_cast< int16 >(value); }

	/* @brief Converts any arithmetic type to uint32 at compile-time.
	* @param value The numeric value to cast.
	* @return The value casted to uint32.
	*/
	template<cArithmetic tType>
	inline constexpr uint32 ToUint32( tType value ) { return static_cast< uint32 >(value); }

	/* @brief Converts any arithmetic type to int32 at compile-time.
	* @param value The numeric value to cast.
	* @return The value casted to int32.
	*/
	template<cArithmetic tType>
	inline constexpr int32 ToInt32( tType value ) { return static_cast< int32 >(value); }

	/* @brief Converts any arithmetic type to uint64 at compile-time.
	* @param value The numeric value to cast.
	* @return The value casted to uint64.
	*/
	template<cArithmetic tType>
	inline constexpr uint64 ToUint64( tType value ) { return static_cast< uint64 >(value); }

	/* @brief Converts any arithmetic type to int64 at compile-time.
	* @param value The numeric value to cast.
	* @return The value casted to int64.
	*/
	template<cArithmetic tType>
	inline constexpr int64 ToInt64( tType value ) { return static_cast< int64 >(value); }

	/* @brief Converts any arithmetic type to float64 at compile-time.
	* @param value The numeric value to cast.
	* @return The value casted to float64.
	*/
	template<cArithmetic tType>
	inline constexpr float64 ToFloat64( tType value ) { return static_cast< float64 >(value); }

	/* @brief Converts any arithmetic type to String at compile-time.
	* @param value The numeric value to cast.
	* @return The value casted to String.
	*/
	template<cArithmetic tType>
	inline constexpr String ToString( tType value ) { return std::to_string( value ); }

	/* @brief Returns the maximum representable value for an arithmetic type.
	* @tparam T Arithmetic type to query.
	* @return std::numeric_limits<tType>::max().
	*/
	template<cArithmetic tType>
	inline constexpr tType MaxValue( ) { return std::numeric_limits<tType>::max( ); }

	/* @brief Returns the minimum representable value for an arithmetic type.
	* @tparam T Arithmetic type to query.
	* @return std::numeric_limits<tType>::min().
	*/
	template<cArithmetic tType>
	inline constexpr tType MinValue( ) { return std::numeric_limits<tType>::min( ); }

	/* @brief Returns the total byte size of a std::vector's contents.
	* @param vector Vector to calculate byte size for.
	* @return sizeof(T) * vector.size().
	*/
	template<typename tType>
	inline constexpr usize ComputeByteSize( const std::vector<tType>& vector ) {
		return sizeof( tType ) * vector.size( );
	}

	/* @brief Returns the total byte size of a raw array.
	* @param arr Reference to the raw array.
	* @return sizeof(T) * L.
	*/
	template<typename tType, usize tL>
	inline constexpr usize ComputeByteSize( const tType( &arr )[ tL ] ) {
		return sizeof( tType ) * tL;
	}

	/* @brief Returns the total byte size of a pointer-based array.
	* @param arr  Pointer to the data.
	* @param size Number of elements.
	* @return sizeof(T) * size.
	*/
	template<typename tType>
	inline constexpr usize ComputeByteSize( const tType* arr, usize size ) {
		return sizeof( tType ) * size;
	}

	/* @brief Returns the element count of a raw array.
	* @param arr Reference to the raw array.
	* @return Number of elements L.
	*/
	template<typename tType, usize tL>
	inline constexpr usize ElementCount( const tType( &arr )[ tL ] ) {
		return tL;
	}

	/* @brief Casts an enum value to its underlying integral type.
	* @param e Enum value to convert.
	* @return The underlying integer value of the enum.
	*/
	template<typename tEnum>
		requires std::is_enum_v<tEnum>
	inline constexpr std::underlying_type_t<tEnum> ToUnderlyingEnum( tEnum e ) {
		return static_cast< std::underlying_type_t<tEnum> >(e);
	}

	/* @brief Hashes a UTF-8 string literal to a 64-bit unsigned integer at compile-time.
	* @tparam L Size of the string literal including null terminator.
	* @param str String literal to hash.
	* @return 64-bit hash of the input string.
	*/
	template<usize tL>
	inline constexpr uint64 HashString( const char( &str )[ tL ] ) noexcept {
		return cstd::StringHasher::Hash( str );
	}

	/* @brief Hashes a StringView to a 64-bit unsigned integer at runtime.
	* @param str String view to hash.
	* @return 64-bit hash of the input string.
	*/
	inline constexpr uint64 HashString( StringView str ) noexcept {
		return cstd::StringHasher::Hash( str );
	}

	/* @brief Converts a String to its lowercase equivalent.
	* @param str Input string to convert.
	* @return New string with all characters lowercased.
	*/
	template<cStringable tType>
	inline String ToLower( tType str ) {
		String result = str.data( );
		std::transform( result.begin( ), result.end( ), result.begin( ), ::tolower );
		return result;
	}

	template<>
	inline String ToLower( char c ) {
		return String( 1, tolower( c ) );
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

	/* @brief Formats a string into a fixed-size stack buffer using printf-style syntax.
	*  @tparam tL     Size of the destination buffer in bytes.
	*  @param buff    Destination character buffer to write into.
	*  @param format  printf-style format string.
	*  @param args    Variadic arguments matching the format specifiers.
	*/
	template<usize tL, typename... tArgs>
	inline void FormatBuffer( char( &buff )[ tL ], ccharptr format, tArgs&&... args ) {
		snprintf( buff, tL, format, std::forward<tArgs>( args )... );
	} 

} // namespace lum