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

		static constexpr usize sk_Npos = usize( -1 );

		/* @brief Constructs a FixedString from a string literal.
		*  @tparam tNewLength Length of the source literal including null terminator.
		*  @param src Source string literal.
		*/
		template<usize tNewLength>
		FixedString( const char( &src )[ tNewLength ] ) noexcept {
			LUM_SASSERT( tNewLength <= tLength && "String is too big" );
			std::memcpy( m_Data, src, tNewLength );
			m_Data[ tNewLength - 1 ] = '\0';
		}

		/* @brief Constructs a FixedString from a null-terminated character string.
		*  @param src Source character string.
		*/
		FixedString( const char* src ) noexcept {
			usize len = strlen( src );
			if (len > tLength - 1) len = tLength - 1;
			std::memcpy( m_Data, src, len );
			m_Data[ len ] = '\0';
		}

		/* @brief Constructs an empty FixedString. */
		FixedString( ) noexcept {
			m_Data[ 0 ] = '\0';
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
			std::memcpy( m_Data, src, len );
			m_Data[ len ] = '\0';
			return *this;
		}

		/* @brief Assigns contents from a character string.
		*  @param src Source character string.
		*  @return Reference to this string.
		*/
		FixedString& operator=( const char* src ) noexcept {
			usize len = strlen( src );
			if (len > tLength) len = tLength - 1;
			std::memcpy( m_Data, src, len );
			m_Data[ len ] = '\0';
			return *this;
		}

		/* @brief Assigns contents from a standard string.
		*  @param src Source string.
		*  @return Reference to this string.
		*/
		FixedString& operator=( const String& src ) noexcept {
			usize len = src.length( );
			if (len > tLength) len = tLength - 1;
			std::memcpy( m_Data, src.data( ), len );
			m_Data[ len ] = '\0';
			return *this;
		}

		/* @brief Appends a null-terminated string.
		*  @param src String to append.
		*  @return Reference to this string.
		*/
		FixedString& operator+=( const char* src ) noexcept {
			usize curLen = strlen( m_Data );
			usize srcLen = strlen( src );
			usize copyLen = std::min( srcLen, tLength - curLen - 1 );
			std::memcpy( m_Data + curLen, src, copyLen );
			m_Data[ curLen + copyLen ] = '\0';
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
			return m_Data[ index ];
		}

		/* @brief Accesses a character by index.
		*  @param index Character index.
		*  @return Const reference to the character.
		*/
		const char& operator[]( usize index ) const {
			return m_Data[ index ];
		}

		/* @brief Returns a character with bounds checking.
		*  @param index Character index.
		*  @return Reference to the character.
		*/
		char& At( usize index ) {
			LUM_ASSERT( index < strlen( m_Data ), "Index out of range" );
			return m_Data[ index ];
		}

		/* @brief Returns a character with bounds checking.
		*  @param index Character index.
		*  @return Const reference to the character.
		*/
		const char& At( usize index ) const {
			LUM_ASSERT( index < strlen( m_Data ), "Index out of range" );
			return m_Data[ index ];
		}

		/* @brief Finds the first occurrence of a character.
		*  @param c Character to search for.
		*  @return Index of the character or FixedString::skNpos if not found.
		*/
		usize Find( char c ) const {
			for (usize i = 0; i < strlen( m_Data ); i++) {
				if (m_Data[ i ] == c) return i;
			}
			return sk_Npos;
		}

		/* @brief Finds the first occurrence of a string view.
		*  @param sv String view to search for.
		*  @return Index of the substring or FixedString::skNpos if not found.
		*/
		usize Find( StringView sv ) const {
			usize size = strlen( m_Data );
			if (sv.size( ) > size) return sk_Npos;

			for (usize i = 0; i <= size - sv.size( ); i++) {
				if (memcmp( m_Data + i, sv.data( ), sv.size( ) ) == 0)
					return i;
			}

			return sk_Npos;
		}

		/* @brief Compares this string against a string literal using hash comparison.
		*  @tparam tNewLength Length of the source literal.
		*  @param src String literal to compare against.
		*  @return True if strings are equal.
		*/
		template<usize tNewLength>
		bool operator==( const char( &src )[ tNewLength ] ) const noexcept {
			return HashString( src ) == HashString( m_Data );
		}

		/* @brief Compares this string against another FixedString using hash comparison.
		*  @tparam tNewLength Size of the other string.
		*  @param oth String to compare against.
		*  @return True if strings are equal.
		*/
		template<usize tNewLength>
		bool operator==( const FixedString<tNewLength>& oth ) const noexcept {
			return HashString( oth.m_Data ) == HashString( m_Data );
		}

		/* @brief Clears the string contents. */
		constexpr void Clear( ) noexcept {
			m_Data[ 0 ] = '\0';
		}

		/* @brief Returns current string length.
		*  @return Number of characters excluding null terminator.
		*/
		LUM_FORCEINLINE constexpr usize Length( ) noexcept {
			return strlen( m_Data );
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
		LUM_FORCEINLINE constexpr const char* Data( ) const {
			return m_Data;
		}

		/* @brief Returns a writable pointer to the internal character buffer.
		*  @return Pointer to modifiable data.
		*/
		LUM_FORCEINLINE constexpr char* Data( ) {
			return m_Data;
		}

		char* begin( ) { return m_Data; }
		char* end( ) { return m_Data + strlen( m_Data ); }
		const char* begin( ) const { return m_Data; }
		const char* end( ) const { return m_Data + strlen( m_Data ); }

	private:

		/* @brief Internal fixed-size character storage. */
		char m_Data[ tLength ]{};

	};


	/* @brief Non-owning view into an existing fixed string buffer. */
	struct FixedStringView {};

} // namespace lum