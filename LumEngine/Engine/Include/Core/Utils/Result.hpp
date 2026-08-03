#pragma once

#include "Core/Types.hpp"
#include "Core/Utils/Optional.hpp"

namespace lum {

	template<typename tType>
	class LUM_API Result {
	public:

		Result( const tType& value ) : mValue( value ) { }
		Result( tType&& value ) : mValue( std::move( value ) ) { }
		Result() { }
		
		static Result Failure( const String& msg ) {
			
			Result r;
			r.mErrorMsg = msg;
			return r;

		}

		LUM_FORCEINLINE bool HasValue( ) const {

			return mValue.HasValue( );

		}
		LUM_FORCEINLINE const String& Error( ) const {

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

			return HasValue( );

		}

	private:

		Optional<tType> mValue{};
		String mErrorMsg = "";

	};

}