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
		Path( FixedString<tL> path ) : mPath( path.Data() ) { }
		Path( const char* path ) : mPath( path ) { }
		Path( const StringView& path ) : mPath( path ) { }
		Path( const String& path ) : mPath( path ) { }
		Path( const std::filesystem::path& path ) : mPath( path ) { }
		Path() { }

		LUM_FORCEINLINE String ToString( ) const {
			return mPath.string( );
		}

		LUM_FORCEINLINE String Extension( ) const {
			return mPath.extension( ).string( );
		}

		/* @brief Returns the filename without its extension. */
		LUM_FORCEINLINE String Stem( ) const {
			return mPath.stem( ).string( );
		}

		LUM_FORCEINLINE Path Filename( ) const {
			return mPath.filename( );
		}

		LUM_FORCEINLINE Path ParentPath( ) const {
			return mPath.parent_path( );
		}

		/* @brief Returns a normalized path with redundant elements removed. */
		LUM_FORCEINLINE Path LexicallyNormal( ) const {
			return mPath.lexically_normal( );
		}

		LUM_FORCEINLINE Path RelativePath( ) const {
			return mPath.relative_path( );
		}

		LUM_FORCEINLINE bool IsAbsolute( ) const {
			return mPath.is_absolute( );
		}

		LUM_FORCEINLINE bool IsDirectory( ) const {
			return std::filesystem::is_directory( mPath );
		}

		/* @brief Checks whether the path refers to a regular file. */
		LUM_FORCEINLINE bool IsRegularFile( ) const {
			return std::filesystem::is_regular_file( mPath );
		}

		LUM_FORCEINLINE bool HasExtension( ) const {
			return mPath.has_extension( );
		}

		LUM_FORCEINLINE bool Empty( ) const {
			return mPath.empty( );
		}

		LUM_FORCEINLINE bool EndsWith( const StringView& str ) const {
			return mPath.string( ).ends_with( str );
		}

		/* @brief Returns the hash of the path string. */
		LUM_FORCEINLINE uint64 Hash( ) const {
			return HashString( ToString( ) );
		}

		LUM_FORCEINLINE Path& ReplaceExtension( const StringView& extension ) {
			mPath.replace_extension( extension ); 
			return *this; 
		}

		Path operator/( const Path& other ) const {
			return mPath / other.mPath;
		}
		Path operator/( const String& other ) const {
			return mPath / other;
		}
		Path operator/( const StringView& other ) const {
			return mPath / other;
		}
		Path operator/( const char* other ) const {
			return mPath / other;
		}

		Path& operator/=( const Path& other ) {
			mPath /= other.mPath;
			return *this;
		}
		Path& operator/=( const String& other ) {
			mPath /= other;
			return *this;
		}
		Path& operator/=( const StringView& other ) {
			mPath /= other;
			return *this;
		}
		Path& operator/=( const char* other ) {
			mPath /= other;
			return *this;
		}

		Path& operator=( const String& other ) {
			mPath = other;
			return *this;
		}
		Path& operator=( StringView other ) {
			mPath = other;
			return *this;
		}
		Path& operator=( const char* other ) {
			mPath = other;
			return *this;
		}

		bool operator==( const Path& other ) const {
			return mPath == other.mPath;
		}

	private:

		friend class FileSystem;

		std::filesystem::path mPath{};

	};

}