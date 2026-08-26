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

		/* @brief Reads the entire contents of a text file. */
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
				FormatString( buff, "Failed to write file '%s': '%s'", path.ToString( ).c_str( ), strerror( errno ) );
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