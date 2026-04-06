//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Fixed-size stack-allocated string with no heap allocation.
//
//=============================================================================//
#pragma once
#include "core/core_common.hpp"

namespace lum {

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
		explicit FixedString( const char(&src)[tNewL] ) noexcept {
			LUM_SASSERT(tNewL <= tL && "String is too big");
			mSize = tNewL - 1;
			std::memcpy(mBuffer, src, tNewL);
		}

		explicit FixedString( ) noexcept {
			mBuffer[0] = '\0';
		}

		template<usize tNewL>
		FixedString& operator=( const char(&src)[tNewL] ) noexcept {
			usize len = tNewL;
			if (len > tL) len = tL;
			mSize = len;
			std::memcpy(mBuffer, src, len);
			mBuffer[len] = '\0';
			return *this;
		}

		FixedString& operator=( const char* src ) noexcept {
			usize len = std::strlen(src);
			if (len > tL) len = tL;
			mSize = len;
			std::memcpy(mBuffer, src, std::min(len, tL));
			mBuffer[len] = '\0';
			return *this;
		}

		FixedString& operator=( const String& src ) noexcept {
			usize len = src.length( );
			if (len > tL) len = tL;
			mSize = len;
			std::memcpy( mBuffer, src.data( ), std::min( len, tL ) );
			mBuffer[ len ] = '\0';
			return *this;
		}

		/* @brief Compares this string against a string literal using hash comparison.
		*  @tparam tNewL Length of the source literal.
		*  @param src String literal to compare against.
		*/
		template<usize tNewL>
		bool operator==( const char(&src)[tNewL] ) const noexcept {
			if (HashStr(src) == HashStr(mBuffer))
				return true;
			return false;
		}

		/* @brief Compares this string against another FixedString using hash comparison.
		*  @param oth FixedString to compare against.
		*/
		template<usize tNewL>
		bool operator==( const FixedString& oth ) const noexcept {
			if (HashStr(oth.mBuffer) == HashStr(mBuffer))
				return true;
			return false;
		}

		/* @brief Clears the string, resetting length to zero. */
		constexpr void Clear( ) noexcept {
			mSize = 0;
			mBuffer[0] = '\0';
		}

		/* @brief Returns the current length of the string, excluding null terminator. */
		LUM_FORCEINLINE constexpr usize Length( )  noexcept { return mSize; }

		/* @brief Returns the maximum number of characters this string can hold. */
		LUM_FORCEINLINE constexpr usize MaxSize( ) noexcept { return tL; }

		/* @brief Returns a pointer to the underlying null-terminated character buffer. */
		LUM_FORCEINLINE constexpr const char* Data( ) const { return mBuffer; }

		LUM_FORCEINLINE constexpr char* Data( ) { return mBuffer; }

	private:

		/* @brief Internal character buffer of fixed compile-time size. */
		char mBuffer[tL]{};

		/* @brief Current number of characters stored, excluding null terminator. */
		usize mSize = 0;

	};

	// TO IMPLEMENT:
	/* @brief Non-owning view into an existing string buffer. */
	struct FixedStringView {
	};

} // namespace lum