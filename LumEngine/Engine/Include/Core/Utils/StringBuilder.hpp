//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Dynamic string builder helper class for Lum engine.
//
//=============================================================================//
#pragma once

#include "Core/Types.hpp"
#include "Core/CoreCommon.hpp"

namespace lum {

	/* @brief High-performance dynamic string construction buffer.
	*
	* Provides a zero-overhead fluent interface for accumulating text,
	* formatting primitive values, and preventing unnecessary string reallocations.
	*/
	class StringBuilder {
	public:

		constexpr StringBuilder( ) noexcept = default;

		/* @brief Constructs StringBuilder and pre-allocates internal memory buffer.
		* @param capacity Initial capacity in bytes to reserve.
		*/
		explicit StringBuilder( usize capacity ) {
			m_Buffer.reserve( capacity );
		}

		/* @brief Appends a string view slice to the buffer.
		* @param str String slice to append.
		*/
		void Append( StringView str ) {
			m_Buffer.append( str );
		}

		/* @brief Appends a null-terminated C-style string to the buffer.
		* @param str Pointer to null-terminated char array.
		*/
		void Append( const char* str ) {
			if (str) {
				m_Buffer.append( str );
			}
		}

		/* @brief Appends a single character byte to the buffer.
		* @param ch Character to append.
		*/
		void Append( char ch ) {
			m_Buffer.push_back( ch );
		}

		/* @brief Formats and appends numeric/arithmetic values as strings.
		* @tparam tType Type constrained by the cArithmetic concept.
		* @param value Numeric value to format.
		*/
		template<cArithmetic tType>
		void Append( tType value ) {
			m_Buffer.append( lum::ToString( value ) );
		}

		/* @brief Appends a newline character ('\n') to the buffer. */
		void AppendLine( ) {
			m_Buffer.push_back( '\n' );
		}

		/* @brief Appends a string view slice followed by a newline character.
		* @param str String slice to append.
		*/
		void AppendLine( StringView str ) {
			m_Buffer.append( str );
			m_Buffer.push_back( '\n' );
		}

		/* @brief Appends a null-terminated C-style string followed by a newline character.
		* @param str Pointer to null-terminated char array.
		*/
		void AppendLine( const char* str ) {
			if (str) {
				m_Buffer.append( str );
			}
			m_Buffer.push_back( '\n' );
		}

		/* @brief Formats and appends a numeric value followed by a newline character.
		* @tparam tType Type constrained by the cArithmetic concept.
		* @param value Numeric value to format.
		*/
		template<cArithmetic tType>
		void AppendLine( tType value ) {
			m_Buffer.append( lum::ToString( value ) );
			m_Buffer.push_back( '\n' );
		}

		/* @brief Clears the internal string buffer without deallocating reserved memory. */
		void Clear( ) noexcept {
			m_Buffer.clear( );
		}

		/* @brief Returns the total count of characters currently stored in the buffer. */
		LUM_NODISCARD usize Length( ) const noexcept {
			return m_Buffer.length( );
		}

		/* @brief Checks whether the internal buffer contains zero characters. */
		LUM_NODISCARD bool IsEmpty( ) const noexcept {
			return m_Buffer.empty( );
		}

		/* @brief Constructs a non-allocating StringView over the internal character array. */
		LUM_NODISCARD StringView View( ) const noexcept {
			return StringView( m_Buffer );
		}

		/* @brief Rvalue overload: moves internal string buffer out of temporary objects (zero allocations). */
		LUM_NODISCARD String ToString( ) && noexcept {
			return std::move( m_Buffer );
		}

		/* @brief Lvalue overload: returns a read-only const reference to internal string buffer. */
		LUM_NODISCARD const String& ToString( ) const& noexcept {
			return m_Buffer;
		}

		/* @brief Implicit conversion operator to StringView for transparent view passing. */
		LUM_NODISCARD operator StringView( ) const noexcept {
			return View( );
		}

	private:

		String m_Buffer{};

	};

} // namespace lum