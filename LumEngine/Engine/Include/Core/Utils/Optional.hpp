//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Stores optional value with explicit checking and safety assertions.
//
//=============================================================================//
#pragma once

#include "Core/CoreCommon.hpp"

namespace lum {

	/*
	* @brief Wrapper class representing an optional value that may or may not exist.
	* @tparam tType The underlying value type to store.
	*/
	template<typename tType>
	class LUM_API Optional {
	public:

		/*
		* @brief Constructs an empty Optional object.
		*/
		Optional( ) : mValue( std::nullopt ) { }

		Optional( const tType& value ) : mValue( value ) { }
		Optional( tType&& value ) : mValue( std::move( value ) ) { }

		Optional( const Optional& ) = default;
		Optional( Optional&& ) = default;
		Optional& operator=( const Optional& ) = default;
		Optional& operator=( Optional&& ) = default;

		/*
		* @brief Creates a static instance of an empty Optional object.
		* @return An uninitialized Optional instance.
		*/
		static Optional Empty( ) {
			return Optional( );
		}

		LUM_FORCEINLINE bool HasValue( ) const {
			return mValue.has_value( );
		}

		/*
		* @brief Accesses the contained value.
		* @return Reference to the underlying value.
		* Asserts at runtime in Debug builds if no value is set.
		*/
		LUM_FORCEINLINE tType& Value( ) {
			LUM_ASSERT( HasValue( ), "Optional has no value." );
			return *mValue;
		}

		/*
		* @brief Accesses the contained value as a const reference.
		* @return Const reference to the underlying value.
		* Asserts at runtime in Debug builds if no value is set.
		*/
		LUM_FORCEINLINE const tType& Value( ) const {
			LUM_ASSERT( HasValue( ), "Optional has no value." );
			return *mValue;
		}

		/*
		* @brief Resets the Optional instance back to an empty state.
		*/
		LUM_FORCEINLINE void Reset( ) {
			mValue.reset( );
		}

		explicit operator bool( ) const {
			return HasValue( );
		}

		tType& operator*( ) {
			return Value( );
		}

		const tType& operator*( ) const {
			return Value( );
		}

		tType* operator->( ) {
			return &Value( );
		}

		const tType* operator->( ) const {
			return &Value( );
		}

	private:

		std::optional<tType> mValue; ///< Internal std::optional container instance.

	};

} // namespace lum