#pragma once

#include "Core/CoreCommon.hpp"
#include "Core/Utils/Result.hpp"

#include "Platform/Path.hpp"

namespace lum {

	class LUM_API FileSystem {
	public:

		static bool Exists( const Path& path ) {

			return std::filesystem::exists( path.ToString( ) );

		}

		static Result<String> ReadAllText( const Path& path ) {

			std::ifstream file( path.mPath );
			if (!file.is_open( )) {
				char buff[ 512 ]{};
				FormatString( buff, "Failed to read file '%s': '%s'", path.ToString( ).c_str( ), strerror( errno ) );
				return Result<String>::Failure( buff );
			}

			std::ostringstream ss;
			ss << file.rdbuf( );

			file.close( );

			return ss.str( );

		}

		static Result<bool> WriteAllText( const Path& path, const String& content ) {

			std::ofstream file( path.mPath );
			if (!file.is_open( )) {
				char buff[ 512 ]{};
				FormatString( buff, "Failed to write file '%s': '%s'", path.ToString( ).c_str( ), strerror( errno ) );
				return Result<bool>::Failure( buff );
			}

			file << content;
			file.close( );

			return true;

		}

		static Path CurrentPath( ) {
			return std::filesystem::current_path( );
		}



	};

}