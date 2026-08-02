//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Defines a fixed-size stack-allocated string type with compile-time
//          capacity, string manipulation utilities, comparison operations,
//          searching and direct character buffer access.
//
//=============================================================================//
#pragma once

#include "Core/CoreCommon.hpp"

namespace lum {

	/* @brief Stack-allocated string with a compile-time maximum length.
	*  Never allocates heap memory — all storage lives in a fixed-size character buffer.
	*  Provides assignment, concatenation, comparison, searching and raw buffer access.
	*  @tparam tLength Maximum number of characters the string can store excluding null terminator.
	*/
	template<usize tLength>
	struct FixedString {

		static constexpr usize skNpos = usize( -1 );

		/* @brief Constructs a FixedString from a string literal.
		*  @tparam tNewLength Length of the source literal including null terminator.
		*  @param src Source string literal.
		*/
		template<usize tNewLength>
		FixedString( const char( &src )[ tNewLength ] ) noexcept {
			LUM_SASSERT( tNewLength <= tLength && "String is too big" );
			std::memcpy( mData, src, tNewLength );
			mData[ tNewLength - 1 ] = '\0';
		}

		/* @brief Constructs a FixedString from a null-terminated character string.
		*  @param src Source character string.
		*/
		FixedString( ccharptr src ) noexcept {
			usize len = strlen( src );
			if (len > tLength - 1) len = tLength - 1;
			std::memcpy( mData, src, len );
			mData[ len ] = '\0';
		}

		/* @brief Constructs an empty FixedString. */
		FixedString( ) noexcept {
			mData[ 0 ] = '\0';
		}

		/* @brief Assigns contents from a string literal.
		*  @tparam tNewLength Length of the source literal.
		*  @param src Source string literal.
		*  @return Reference to this string.
		*/
		template<usize tNewLength>
		FixedString& operator=( const char( &src )[ tNewLength ] ) noexcept {
			usize len = tNewLength;
			if (len > tLength) len = tLength - 1;
			std::memcpy( mData, src, len );
			mData[ len ] = '\0';
			return *this;
		}

		/* @brief Assigns contents from a character string.
		*  @param src Source character string.
		*  @return Reference to this string.
		*/
		FixedString& operator=( ccharptr src ) noexcept {
			usize len = strlen( src );
			if (len > tLength) len = tLength - 1;
			std::memcpy( mData, src, len );
			mData[ len ] = '\0';
			return *this;
		}

		/* @brief Assigns contents from a standard string.
		*  @param src Source string.
		*  @return Reference to this string.
		*/
		FixedString& operator=( const String& src ) noexcept {
			usize len = src.length( );
			if (len > tLength) len = tLength - 1;
			std::memcpy( mData, src.data( ), len );
			mData[ len ] = '\0';
			return *this;
		}

		/* @brief Appends a null-terminated string.
		*  @param src String to append.
		*  @return Reference to this string.
		*/
		FixedString& operator+=( ccharptr src ) noexcept {
			usize curLen = strlen( mData );
			usize srcLen = strlen( src );
			usize copyLen = std::min( srcLen, tLength - curLen - 1 );
			std::memcpy( mData + curLen, src, copyLen );
			mData[ curLen + copyLen ] = '\0';
			return *this;
		}

		/* @brief Appends another FixedString.
		*  @tparam tNewLength Size of the other string.
		*  @param oth String to append.
		*  @return Reference to this string.
		*/
		template<usize tNewLength>
		FixedString& operator+=( const FixedString<tNewLength>& oth ) noexcept {
			return *this += oth.Data( );
		}

		/* @brief Accesses a character by index.
		*  @param index Character index.
		*  @return Reference to the character.
		*/
		char& operator[]( usize index ) {
			return mData[ index ];
		}

		/* @brief Accesses a character by index.
		*  @param index Character index.
		*  @return Const reference to the character.
		*/
		const char& operator[]( usize index ) const {
			return mData[ index ];
		}

		/* @brief Returns a character with bounds checking.
		*  @param index Character index.
		*  @return Reference to the character.
		*/
		char& At( usize index ) {
			LUM_ASSERT( index < strlen( mData ), "Index out of range" );
			return mData[ index ];
		}

		/* @brief Returns a character with bounds checking.
		*  @param index Character index.
		*  @return Const reference to the character.
		*/
		const char& At( usize index ) const {
			LUM_ASSERT( index < strlen( mData ), "Index out of range" );
			return mData[ index ];
		}

		/* @brief Finds the first occurrence of a character.
		*  @param c Character to search for.
		*  @return Index of the character or FixedString::skNpos if not found.
		*/
		usize Find( char c ) const {
			for (usize i = 0; i < strlen( mData ); i++) {
				if (mData[ i ] == c) return i;
			}
			return skNpos;
		}

		/* @brief Finds the first occurrence of a string view.
		*  @param sv String view to search for.
		*  @return Index of the substring or FixedString::skNpos if not found.
		*/
		usize Find( StringView sv ) const {
			usize size = strlen( mData );
			if (sv.size( ) > size) return skNpos;

			for (usize i = 0; i <= size - sv.size( ); i++) {
				if (memcmp( mData + i, sv.data( ), sv.size( ) ) == 0)
					return i;
			}

			return skNpos;
		}

		/* @brief Compares this string against a string literal using hash comparison.
		*  @tparam tNewLength Length of the source literal.
		*  @param src String literal to compare against.
		*  @return True if strings are equal.
		*/
		template<usize tNewLength>
		bool operator==( const char( &src )[ tNewLength ] ) const noexcept {
			return HashString( src ) == HashString( mData );
		}

		/* @brief Compares this string against another FixedString using hash comparison.
		*  @tparam tNewLength Size of the other string.
		*  @param oth String to compare against.
		*  @return True if strings are equal.
		*/
		template<usize tNewLength>
		bool operator==( const FixedString<tNewLength>& oth ) const noexcept {
			return HashString( oth.mData ) == HashString( mData );
		}

		/* @brief Clears the string contents. */
		constexpr void Clear( ) noexcept {
			mData[ 0 ] = '\0';
		}

		/* @brief Returns current string length.
		*  @return Number of characters excluding null terminator.
		*/
		LUM_FORCEINLINE constexpr usize Length( ) noexcept {
			return strlen( mData );
		}

		/* @brief Returns maximum string capacity.
		*  @return Maximum number of stored characters.
		*/
		LUM_FORCEINLINE constexpr usize MaxSize( ) noexcept {
			return tLength;
		}

		/* @brief Returns a pointer to the internal character buffer.
		*  @return Const pointer to null-terminated data.
		*/
		LUM_FORCEINLINE constexpr ccharptr Data( ) const {
			return mData;
		}

		/* @brief Returns a writable pointer to the internal character buffer.
		*  @return Pointer to modifiable data.
		*/
		LUM_FORCEINLINE constexpr charptr Data( ) {
			return mData;
		}

		charptr begin( ) { return mData; }
		charptr end( ) { return mData + strlen( mData ); }
		ccharptr begin( ) const { return mData; }
		ccharptr end( ) const { return mData + strlen( mData ); }

	private:

		/* @brief Internal fixed-size character storage. */
		char mData[ tLength ]{};

	};


	/* @brief Non-owning view into an existing fixed string buffer. */
	struct FixedStringView { };

} // namespace lum