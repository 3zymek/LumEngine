//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Fast, non-allocating string formatting utilities.
//
//=============================================================================//
#pragma once

#include "Core/CorePch.hpp"
#include "Core/Types.hpp"

namespace lum {

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

} // namespace lum