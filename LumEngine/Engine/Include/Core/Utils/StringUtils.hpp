#pragma once

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cstring>
#include <string>
#include <system_error>
#include <utility>

#include "Core/CoreTypes.hpp"

namespace lum {

	/* @brief Converts any arithmetic type to String at compile-time.
	* @param value The numeric value to cast.
	* @return The value casted to String.
	*/
	template<cArithmetic tType>
	inline constexpr String ToString( tType value ) {
		return std::to_string( value );
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

	// Hashed string representation using FNV-1a (64-bit).
	// Use cstd::StringHasher::Hash() to produce values of this type.
	using HashedString = uint64;

	/* @brief Hashes a UTF-8 string literal to a 64-bit unsigned integer at compile-time.
	* @tparam L Size of the string literal including null terminator.
	* @param str String literal to hash.
	* @return 64-bit hash of the input string.
	*/
	template<usize tL>
	inline constexpr HashedString HashString( const char( &str )[ tL ] ) noexcept {
		HashedString h = 14695981039346656037ULL;

		for (usize i = 0; i < tL - 1; ++i) {
			h ^= static_cast<uint8>( str[ i ] );
			h *= 1099511628211ULL;
		}

		return h;
	}

	/* @brief Hashes a StringView to a 64-bit unsigned integer at runtime.
	* @param str String view to hash.
	* @return 64-bit hash of the input string.
	*/
	inline constexpr HashedString HashString( StringView str ) noexcept {
		HashedString h = 14695981039346656037ULL;

		for (char c : str) {
			h ^= static_cast<uint8>(c);
			h *= 1099511628211ULL;
		}

		return h;
	}

	namespace detail {

		/* @brief Appends a string slice (StringView) to the target char buffer.
		*
		* Performs bound-checked memory copying to prevent buffer overflow.
		*
		* @param[in,out] current Reference to current output buffer write head pointer.
		* @param[in] end Pointer to the end of the buffer (excluding null-terminator byte).
		* @param[in] sv StringView containing the text slice to copy.
		*/
		inline void AppendArg( char*& current, const char* end, StringView sv ) {
			if (current >= end) return;

			const usize len = sv.length( );
			const usize spaceLeft = (current < end) ? static_cast<usize>( end - current ) : 0;
			const usize toCopy = (len < spaceLeft) ? len : spaceLeft;

			if (toCopy > 0) {
				std::memcpy( current, sv.data( ), toCopy );
				current += toCopy;
			}
		}

		/* @brief Formats and appends an arithmetic value( integer / float ) into the buffer.
		*
		* Uses high-performance, non-allocating std::to_chars for numeric conversion.
		*
		* @tparam tType Arithmetic type constrained by cArithmetic concept.
		* @param[in,out] current Reference to current output buffer write head pointer.
		* @param[in] end Pointer to the end of the buffer.
		* @param[in] val Value to format into string representation.
		*/
		template<cArithmetic tType>
		inline void AppendArg( char*& current, const char* end, tType val ) {
			if (current >= end) return;

			auto [ptr, error] = std::to_chars( current, const_cast<char*>(end), val );
			if (error == std::errc{}) {
				current = ptr;
			}
		}

		template<cEnum tEnum>
		inline void AppendArg( char*& current, const char* end, tEnum val ) {
			AppendArg( current, end, "DEBUG ENUM_NAME" );
		}

		/* @brief Base recursion terminator for string formatting.
		*
		* Copies remaining literal string text when no dynamic arguments remain.
		*
		* @param[in,out] current Reference to current output buffer write head pointer.
		* @param[in] end Pointer to the end of the buffer.
		* @param[in] fmt Pointer to null-terminated format string.
		*/
		inline void FormatImpl( char*& current, const char* end, const char* fmt ) {
			while (*fmt && current < end) {
				*current++ = *fmt++;
			}
		}

		/* @brief Recursive variadic template worker for string formatting.
		*
		* Scans format string for "{}" placeholders and substitutes them with
		* arguments sequentially using perfect forwarding.
		*
		* @tparam tType Type of current argument to substitute.
		* @tparam tArgs Variadic pack of remaining argument types.
		* @param[in,out] current Reference to current output buffer write head pointer.
		* @param[in] end Pointer to the end of the buffer.
		* @param[in] fmt Pointer to format string.
		* @param[in] arg Current argument to substitute into "{}".
		* @param[in] args Remaining arguments for recursive substitution.
		*/
		template<typename tType, typename... tArgs>
		inline void FormatImpl( char*& current, const char* end, const char* fmt, tType&& arg, tArgs&&... args ) {
			while (*fmt && current < end) {
				// Detect empty "{}" placeholder
				if (fmt[ 0 ] == '{' && fmt[ 1 ] == '}') {
					fmt += 2; // Advance past "{}"
					AppendArg( current, end, std::forward<tType>( arg ) );
					FormatImpl( current, end, fmt, std::forward<tArgs>( args )... );
					return;
				}
				*current++ = *fmt++;
			}
		}

	} // namespace detail



	/* @brief Formats a compile-time char array with variadic arguments.
	*
	* Safer alternative to sprintf_s/snprintf. Ensures zero heap allocations
	* and automatic null-termination within the destination array boundaries.
	*
	* @tparam tL Length of target destination character array (deduced automatically).
	* @tparam tArgs Variadic pack of argument types to format.
	* @param[out] output Fixed-size character array buffer to receive output.
	* @param[in] fmt Format string containing "{}" placeholders.
	* @param[in] args Arguments to substitute into placeholders.
	*/
	template<usize tL, typename... tArgs>
	inline void FormatString( char( &output )[ tL ], const char* fmt, tArgs&&... args ) {
		char* current = output;
		const char* end = output + tL - 1; // Reserve 1 byte for null terminator

		detail::FormatImpl( current, end, fmt, std::forward<tArgs>( args )... );
		*current = '\0'; // Guaranteed null-termination
	}

	/* @brief Formats a compile-time char array with variadic arguments.
	*
	* Safer alternative to sprintf_s/snprintf. Ensures zero heap allocations
	* and automatic null-termination within the destination array boundaries.
	*
	* @tparam tL Length of target destination character array (deduced automatically).
	* @tparam tArgs Variadic pack of argument types to format.
	* @param[out] output FixedString to receive output.
	* @param[in] fmt Format string containing "{}" placeholders.
	* @param[in] args Arguments to substitute into placeholders.
	*/
	/*template<typename... tArgs, usize tLength>
	inline void FormatString( FixedString<tLength>& fixedStr, const char* fmt, tArgs&&... args ) {
		char* current = fixedStr.At( 0 );
		const char* end = fixedStr.At( 0 ) + tLength - 1;

		detail::FormatImpl( current, end, fmt, std::forward<tArgs>( args )... );
		*current = '\0'; // Guaranteed null-termination
	}*/

	/*
	* @brief Formats a string with variadic arguments and returns the result.
	*
	* Allocates a String with additional buffer capacity to accommodate formatted
	* output. Ensures automatic null-termination and safe writing boundaries.
	*
	* @tparam tAdditionalLength Additional buffer capacity beyond the format string length. Defaults to 64 characters.
	* @tparam tArgs Variadic pack of argument types to format.
	* @param[in] fmt Format string containing "{}" placeholders.
	* @param[in] args Arguments to substitute into placeholders.
	* @return String containing the formatted output.
	*/
	template<usize tAdditionalLength = 64, typename... tArgs>
	inline String FormatString( const char* fmt, tArgs&&... args ) {

		String str{};
		str.resize( strlen( fmt ) + tAdditionalLength );

		char* current = str.data( );
		const char* end = current + str.capacity( ) - 1;

		detail::FormatImpl(
			current,
			end,
			fmt,
			std::forward<tArgs>( args )...
		);

		*current = '\0';

		return str;

	}

} // namespace lum