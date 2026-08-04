#pragma once

#include "Core/Types.hpp"
#include "Core/CoreCommon.hpp"

namespace lum {

	class StringBuilder {
	public:

		void Append( StringView str ) {
			mBuffer.append( str );
		}
		void Append( const char* str ) {
			mBuffer.append( str );
		}
		template<typename tType>
		void Append( tType&& value ) {
			mBuffer.append( std::forward<tType>( ToString( value ) ) );
		}

		void AppendLine( StringView str ) {
			mBuffer.append( str );
			mBuffer.push_back( '\n' );
		}
		void AppendLine( const char* str ) {
			mBuffer.append( str );
			mBuffer.push_back( '\n' );
		}
		template<typename tType>
		void AppendLine( tType&& value ) {
			mBuffer.append( std::forward<tType>( ToString( value ) ) );
			mBuffer.push_back( '\n' );
		}

		void AppendLine( ) {
			mBuffer.push_back( '\n' );
		}

		void Clear( ) {
			mBuffer.clear( );
		}

		String ToString( ) {
			return mBuffer;
		}
		const String& ToString( ) const {
			return mBuffer;
		}

	private:

		String mBuffer{};

	};

} // namespace lum