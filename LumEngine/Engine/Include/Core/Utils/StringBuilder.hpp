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
		template<cArithmetic tType>
		void Append( tType value ) {
			mBuffer.append( lum::ToString( value ) );
		}

		void AppendLine( StringView str ) {
			mBuffer.append( str );
			mBuffer.push_back( '\n' );
		}
		void AppendLine( const char* str ) {
			mBuffer.append( str );
			mBuffer.push_back( '\n' );
		}
		template<cArithmetic tType>
		void AppendLine( tType value ) {
			mBuffer.append( lum::ToString( value ) );
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