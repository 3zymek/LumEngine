#pragma once
#include "Core/CoreCommon.hpp"
#include "Core/Utils/FixedString.hpp"

namespace lum {

	class LUM_API Path {
	public:

		Path( StringView path ) : mPath( path.data( ) ) { 
			parse( ); 
		}
		
		LUM_FORCEINLINE String Filename( ) const { return mPath.Data(); }
		LUM_FORCEINLINE String Extension( ) const { return mExtension.Data(); }	
		LUM_FORCEINLINE String String( ) const { return mPath.Data( ); }
		
	private:

		friend class FileSystem;

		FixedString<260> mPath = "";
		FixedString<32> mExtension = "";
		FixedString<64> mFilename = "";

		void parse( ) {

			std::filesystem::path path = mPath.Data( );

			mExtension = path.extension( ).string();
			mFilename = path.filename( ).string( );

		}

	};

}