//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Generic result wrapper representing either a successful value or an error message.
//
//=============================================================================//
#pragma once

#include "Core/Utils/Optional.hpp"

namespace lum {

	/*
	* @brief Represents the result of an operation that can either produce a value of type tType or fail with an error string.
	* @tparam tType The underlying value type on success.
	*/
	template<typename tType>
	class LUM_API Result {
	public:

		/*
		* @brief Constructs a successful Result.
		* @param value The successful value to copy into the result.
		*/
		Result( const tType& value ) : mValue( value ) { }

		/*
		* @brief Constructs a successful Result.
		* @param value The successful value to move into the result.
		*/
		Result( tType&& value ) : mValue( std::move( value ) ) { }

		/*
		* @brief Constructs an empty Result instance (failed by default).
		*/
		Result( ) { }

		/*
		* @brief Constructs a failed Result instance containing an error message.
		* @param msg The error description explaining why the operation failed.
		* @return Result instance in a failed state.
		*/
		static Result Failure( const String& msg ) {
			Result r;
			r.mErrorMsg = msg;
			return r;
		}

		/*
		* @brief Constructs a succeeded Result instance containing a value.
		* @param val Result value.
		* @return Result instance in a succeeded state.
		*/
		static Result Success( const tType& val ) {
			Result r( val );
			return r;
		}
		static Result Success( tType&& val ) {
			Result r( val );
			return r;
		}

		LUM_FORCEINLINE bool IsSuccess( ) const {
			return mValue.HasValue( );
		}

		/*
		* @brief Gets the error message associated with a failed result.
		* @return Const reference to the error message string.
		*/
		LUM_FORCEINLINE const String& GetError( ) const {
			return mErrorMsg;
		}

		/////////////////////////////////
		/// Value getters
		///////////////////////////////// 

		LUM_FORCEINLINE tType* ValuePtr( ) {
			return mValue.HasValue( ) ? &mValue.Value( ) : nullptr;
		}

		LUM_FORCEINLINE const tType* ValuePtr( ) const {
			return mValue.HasValue( ) ? &mValue.Value( ) : nullptr;
		}

		LUM_FORCEINLINE tType& ValueRef( ) {
			return mValue.Value( );
		}

		LUM_FORCEINLINE const tType& ValueRef( ) const {
			return mValue.Value( );
		}

		tType& operator*( ) {
			return mValue.Value( );
		}

		const tType& operator*( ) const {
			return mValue.Value( );
		}

		tType* operator->( ) {
			return &mValue.Value( );
		}

		const tType* operator->( ) const {
			return &mValue.Value( );
		}

		explicit operator bool( ) const {
			return IsSuccess( );
		}

	private:

		Optional<tType> mValue{};  //< Optional container holding the resulting value on success.
		String mErrorMsg = "";     //< Description of the failure if the operation failed.

	};

} // namespace lum