#pragma once

#include "Core/CoreCommon.hpp"
#include "Platform/Path.hpp"

namespace lum {
	
	class LUM_API FileSystem {
		
		static bool Exists( const Path& path ) {

			return std::filesystem::exists( path.String( ) );

		}

		static std::optional<String> Read( const Path& path ) {

			std::ifstream file( path.String( ) );
			if (!file.is_open( )) {
				LUM_LOG_ERROR( "Failed to read file '%s': '%s'", path.String( ).c_str( ), strerror( errno ) );
				return std::nullopt;
			}

			std::ostringstream ss;
			ss << file.rdbuf( );

			file.close( );

			return ss.str( );

		}
			
		
		
	};

}