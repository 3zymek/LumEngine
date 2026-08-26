//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Provides a path abstraction for filesystem operations.
//
//=============================================================================//
#pragma once

#include "Core/CoreCommon.hpp"
#include "Core/Utils/FixedString.hpp"

namespace lum {

	class LUM_API Path {
	public:

		template<usize tL>
		Path( FixedString<tL> path ) : m_Path( path.Data() ) { }
		Path( const char* path ) : m_Path( path ) { }
		Path( const StringView& path ) : m_Path( path ) { }
		Path( const String& path ) : m_Path( path ) { }
		Path( const std::filesystem::path& path ) : m_Path( path ) { }
		Path() { }

		LUM_FORCEINLINE String ToString( ) const {
			return m_Path.string( );
		}

		LUM_FORCEINLINE String Extension( ) const {
			return m_Path.extension( ).string( );
		}

		/* @brief Returns the filename without its extension. */
		LUM_FORCEINLINE String Stem( ) const {
			return m_Path.stem( ).string( );
		}

		LUM_FORCEINLINE Path Filename( ) const {
			return m_Path.filename( );
		}

		LUM_FORCEINLINE Path ParentPath( ) const {
			return m_Path.parent_path( );
		}

		/* @brief Returns a normalized path with redundant elements removed. */
		LUM_FORCEINLINE Path LexicallyNormal( ) const {
			return m_Path.lexically_normal( );
		}

		LUM_FORCEINLINE Path RelativePath( ) const {
			return m_Path.relative_path( );
		}

		LUM_FORCEINLINE bool IsAbsolute( ) const {
			return m_Path.is_absolute( );
		}

		LUM_FORCEINLINE bool IsDirectory( ) const {
			return std::filesystem::is_directory( m_Path );
		}

		/* @brief Checks whether the path refers to a regular file. */
		LUM_FORCEINLINE bool IsRegularFile( ) const {
			return std::filesystem::is_regular_file( m_Path );
		}

		LUM_FORCEINLINE bool HasExtension( ) const {
			return m_Path.has_extension( );
		}

		LUM_FORCEINLINE bool Empty( ) const {
			return m_Path.empty( );
		}

		LUM_FORCEINLINE bool EndsWith( const StringView& str ) const {
			return m_Path.string( ).ends_with( str );
		}

		/* @brief Returns the hash of the path string. */
		LUM_FORCEINLINE uint64 Hash( ) const {
			return HashString( ToString( ) );
		}

		LUM_FORCEINLINE Path& ReplaceExtension( const StringView& extension ) {
			m_Path.replace_extension( extension ); 
			return *this; 
		}

		Path operator/( const Path& other ) const {
			return m_Path / other.m_Path;
		}
		Path operator/( const String& other ) const {
			return m_Path / other;
		}
		Path operator/( const StringView& other ) const {
			return m_Path / other;
		}
		Path operator/( const char* other ) const {
			return m_Path / other;
		}

		Path& operator/=( const Path& other ) {
			m_Path /= other.m_Path;
			return *this;
		}
		Path& operator/=( const String& other ) {
			m_Path /= other;
			return *this;
		}
		Path& operator/=( const StringView& other ) {
			m_Path /= other;
			return *this;
		}
		Path& operator/=( const char* other ) {
			m_Path /= other;
			return *this;
		}

		Path& operator=( const String& other ) {
			m_Path = other;
			return *this;
		}
		Path& operator=( StringView other ) {
			m_Path = other;
			return *this;
		}
		Path& operator=( const char* other ) {
			m_Path = other;
			return *this;
		}

		bool operator==( const Path& other ) const {
			return m_Path == other.m_Path;
		}

	private:

		friend class FileSystem;

		std::filesystem::path m_Path{};

	};

}