#pragma once

#include "Core/CorePch.hpp"
#include "Core/Types.hpp"

namespace lum {

	namespace detail {

		inline void AppendArg( char*& current, const char* end, StringView sv ) {
			if (current >= end) return;
			usize len = sv.length( );
			usize spaceLeft = (current < end) ? static_cast<usize>( end - current ) : 0;
			usize toCopy = (len < spaceLeft) ? len : spaceLeft;

			if (toCopy > 0) {
				std::memcpy( current, sv.data( ), toCopy );
				current += toCopy;
			}
		}

		template<cArithmetic tType>
		inline void AppendArg( char*& current, const char* end, tType val ) {
			if (current >= end) return;
			auto [ptr, error] = std::to_chars( current, const_cast<char*>(end), val );
			if (error == std::errc{}) {
				current = ptr;
			}
		}

		inline void FormatImpl( char*& current, const char* end, const char* fmt ) {
			while (*fmt && current < end) {
				*current++ = *fmt++;
			}
		}
		
		template<typename tType, typename... tArgs>
		inline void FormatImpl( char*& current, const char* end, const char* fmt, tType&& arg, tArgs&&... args ) {
			while (*fmt && current < end) {
				if (fmt[ 0 ] == '{' && fmt[ 1 ] == '}') {
					fmt += 2;
					AppendArg( current, end, std::forward<tType>( arg ) );
					FormatImpl( current, end, fmt, std::forward<tArgs>( args )... );
					return;
				}
				*current++ = *fmt++;
			}
		}

	}

	template<usize tL, typename... tArgs>
	void FormatString( char( &buff )[ tL ], const char* fmt, tArgs&&... args ) {
		char* current = buff;
		const char* end = buff + tL - 1;

		detail::FormatImpl( current, end, fmt, std::forward<tArgs>(args )...);
		*current = '\0';

	}

}