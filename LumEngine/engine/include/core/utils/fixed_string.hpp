//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Fixed-size stack-allocated string with no heap allocation.
//
//=============================================================================//
#pragma once
#include "core/core_common.hpp"

namespace lum {

	static constexpr usize skStrNpos = usize( -1 );

	/* @brief Stack-allocated string with a compile-time maximum length.
	*  Never allocates heap memory — all storage lives in a fixed-size char buffer.
	*  @tparam tL Maximum number of characters the string can hold, excluding null terminator.
	*/
	template<usize tL>
	struct FixedString {

		/* @brief Constructs a FixedString from a string literal.
		*  Triggers a static assert if the literal exceeds the maximum length.
		*  @tparam tNewL Length of the source literal including null terminator.
		*  @param src Source string literal.
		*/
		template<usize tNewL>
		FixedString( const char( &src )[ tNewL ] ) noexcept {
			LUM_SASSERT( tNewL <= tL && "String is too big" );
			std::memcpy( mData, src, tNewL );
		}
		FixedString( ccharptr src ) noexcept {
			usize len = strlen( src );
			if (len > tL - 1) len = tL - 1;
			std::memcpy( mData, src, len );
			mData[ len ] = '\0';
		}

		FixedString( ) noexcept {
			mData[ 0 ] = '\0';
		}

		template<usize tNewL>
		FixedString& operator=( const char( &src )[ tNewL ] ) noexcept {
			usize len = tNewL;
			if (len > tL) len = tL - 1;
			std::memcpy( mData, src.data( ), len );
			mData[ len ] = '\0';
			return *this;
		}

		FixedString& operator=( ccharptr src ) noexcept {
			usize len = strlen( src );
			if (len > tL) len = tL - 1;
			std::memcpy( mData, src, len );
			mData[ len ] = '\0';
			return *this;
		}
		FixedString& operator=( const String& src ) noexcept {
			usize len = src.length( );
			if (len > tL) len = tL - 1;
			std::memcpy( mData, src.data( ), len );
			mData[ len ] = '\0';
			return *this;
		}
		template<usize tNewL>
		bool operator!=( const char( &src )[ tNewL ] ) const noexcept {
			return !(*this == src);
		}
		template<usize tNewL>
		bool operator!=( const FixedString<tNewL>& oth ) const noexcept {
			return !(*this == oth);
		}
		bool operator==( ccharptr src ) const noexcept {
			return HashStr( src ) == HashStr( mData );
		}
		bool operator!=( ccharptr src ) const noexcept {
			return !(*this == src);
		}
		bool operator==( StringView sv ) const noexcept {
			return HashStr( sv.data( ) ) == HashStr( mData );
		}
		operator StringView( ) const noexcept { return StringView( mData, strlen( mData ) ); }

		FixedString& operator+=( ccharptr src ) noexcept {
			usize curLen = strlen( mData );
			usize srcLen = strlen( src );
			usize copyLen = std::min( srcLen, tL - curLen - 1 );
			std::memcpy( mData + curLen, src, copyLen );
			mData[ curLen + copyLen ] = '\0';
			return *this;
		}
		template<usize tNewL>
		FixedString& operator+=( const FixedString<tNewL>& oth ) noexcept {
			return *this += oth.Data( );
		}

		char& operator[]( usize index ) {
			return mData[ index ];
		}
		const char& operator[]( usize index ) const {
			return mData[ index ];
		}

		char& At( usize index ) {
			LUM_ASSERT( index < strlen( mData ), "Index out of range" );
			return mData[ index ];
		}
		const char& At( usize index ) const {
			LUM_ASSERT( index < strlen( mData ), "Index out of range" );
			return mData[ index ];
		}

		usize Find( char c ) const {
			for (usize i = 0; i < strlen( mData ); i++) {
				if (mData[ i ] == c) return i;
			}
			return skStrNpos;
		}

		usize Find( StringView sv ) const {
			usize size = strlen( mData );
			if (sv.size( ) > size) return skStrNpos;
			for (usize i = 0; i <= size - sv.size( ); i++) {
				if (memcmp( mData + i, sv.data( ), sv.size( ) ) == 0) return i;
			}
			return skStrNpos;
		}

		/* @brief Compares this string against a string literal using hash comparison.
		*  @tparam tNewL Length of the source literal.
		*  @param src String literal to compare against.
		*/
		template<usize tNewL>
		bool operator==( const char( &src )[ tNewL ] ) const noexcept {
			if (HashStr( src ) == HashStr( mData ))
				return true;
			return false;
		}

		/* @brief Compares this string against another FixedString using hash comparison.
		*  @param oth FixedString to compare against.
		*/
		template<usize tNewL>
		bool operator==( const FixedString& oth ) const noexcept {
			if (HashStr( oth.mData ) == HashStr( mData ))
				return true;
			return false;
		}

		/* @brief Clears the string, resetting length to zero. */
		constexpr void Clear( ) noexcept {
			mData[ 0 ] = '\0';
		}

		/* @brief Returns the current length of the string, excluding null terminator. */
		LUM_FORCEINLINE constexpr usize Length( )  noexcept { return strlen(mData); }

		/* @brief Returns the maximum number of characters this string can hold. */
		LUM_FORCEINLINE constexpr usize MaxSize( ) noexcept { return tL; }

		/* @brief Returns a pointer to the underlying null-terminated character buffer. */
		LUM_FORCEINLINE constexpr ccharptr Data( ) const { return mData; }

		LUM_FORCEINLINE constexpr charptr Data( ) { return mData; }

		charptr begin( ) { return mData; }
		charptr end( ) { return mData + strlen( mData ); }
		ccharptr begin( ) const { return mData; }
		ccharptr end( ) const { return mData + strlen( mData ); }

	private:

		/* @brief Internal character buffer of fixed compile-time size. */
		char mData[ tL ]{};

	};

	// TO IMPLEMENT:
	/* @brief Non-owning view into an existing string buffer. */
	struct FixedStringView { };

} // namespace lum