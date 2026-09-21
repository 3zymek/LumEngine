//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Provides filesystem utilities and abstractions.
//
//=============================================================================//
#pragma once

#include "Core/CoreCommon.hpp"
#include "Core/Utils/Result.hpp"

#include "Platform/FileSystem/Path.hpp"

namespace lum {

	/* @brief Provides utilities for filesystem operations. */
	class LUM_API FileSystem {
	public:

		/* @brief Checks whether the specified path exists. */
		static bool Exists( const Path& path ) {

			return std::filesystem::exists( path.ToString( ) );

		}

		static Result<std::vector<uint32>> ReadAllBytes( const Path& path ) {

			std::ifstream file( path.m_Path, std::ios::binary | std::ios::ate );
			if (!file.is_open( )) {
				char buff[ 512 ]{};
				FormatString( buff, "Failed to read file '{}': '{}'", path.ToString( ), strerror( errno ) );
				return Result<std::vector<uint32>>::Failure( buff );
			}
			
			usize size = static_cast<usize>( file.tellg( ) );
			
			file.seekg( 0 );
			std::vector<uint32> bytes( size / sizeof( uint32 ) );
			file.read( reinterpret_cast<char*>(bytes.data( )), size );
			
			file.close( );

			return bytes;

		}

		static Result<String> ReadAllText( const Path& path ) {

			std::ifstream file( path.m_Path );
			if (!file.is_open( )) {
				char buff[ 512 ]{};
				FormatString( buff, "Failed to read file '{}': '{}'", path.ToString( ), strerror( errno ) );
				return Result<String>::Failure( buff );
			}

			std::ostringstream ss;
			ss << file.rdbuf( );

			file.close( );

			return ss.str( );

		}

		/* @brief Writes the specified content to a text file. */
		static Result<bool> WriteAllText( const Path& path, const String& content ) {

			std::ofstream file( path.m_Path );
			if (!file.is_open( )) {
				char buff[ 512 ]{};
				FormatString( buff, "Failed to write file '{}': '{}'", path.ToString( ).c_str( ), strerror( errno ) );
				return Result<bool>::Failure( buff );
			}

			file << content;
			file.close( );

			return true;

		}

		/* @brief Returns the current working directory. */
		static Path CurrentPath( ) {
			return std::filesystem::current_path( );
		}

	};

}
