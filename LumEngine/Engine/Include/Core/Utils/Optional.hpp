#pragma once

#include "Core/CoreCommon.hpp"

namespace lum {

	template<typename tType>
	class LUM_API Optional {
	public:

		Optional( ) : mValue( std::nullopt ) { }

		Optional( const tType& value ) : mValue( value ) { }
		Optional( tType&& value ) : mValue( std::move( value ) ) { }

		Optional( const Optional& ) = default;
		Optional( Optional&& ) = default;
		Optional& operator=( const Optional& ) = default;
		Optional& operator=( Optional&& ) = default;

		static Optional Empty( ) {
			return Optional( );
		}

		LUM_FORCEINLINE bool HasValue( ) const {

			return mValue.has_value( );

		}
		LUM_FORCEINLINE tType& Value( ) {

			LUM_ASSERT( HasValue( ), "Optional has no value." );
			return mValue.value( );

		}
		LUM_FORCEINLINE const tType& Value( ) const {

			LUM_ASSERT( HasValue( ), "Optional has no value." );
			return mValue.value( );

		}
		LUM_FORCEINLINE void Reset( ) {

			mValue.reset( );

		}
		explicit operator bool( ) const {

			return HasValue( );

		}

	private:

		std::optional<tType> mValue;

	};

}