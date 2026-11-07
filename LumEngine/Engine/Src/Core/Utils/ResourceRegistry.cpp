/*******************************************************/
/* /                                                 \ */
/*|-------------------|LumEngine|---------------------|*/
/*|            Cross-Platform Game Engine             |*/
/*|===================================================|*/
/*|                    GitHub:                        |*/
/*|       https://github.com/3zymek/LumEngine         |*/
/*|                                                   |*/
/*|        Copyright (c) 2025-present 3zymek          |*/
/*|          Licensed under the MIT License           |*/
/* \                                                 / */
/*******************************************************/
#include "Core/CorePch.hpp"

// Header impl file
#include "Core/Utils/ResourceRegistry.hpp"

#include "Core/CoreTypes.hpp"
#include "Core/Limits.hpp"
#include "Core/Utils/HandlePool.hpp"
#include "Core/Utils/LogMacros.hpp"
#include "Core/Utils/Optional.hpp"
#include "Core/Utils/ResourceLoader.hpp"
#include "Core/Utils/StringUtils.hpp"
#include "Platform/FileSystem/Path.hpp"

namespace lum {

	ResourceImageHandle ResourceRegistry::LoadImageFromFile( const Path& path ) noexcept {

		if (path.IsDirectory( )) {
			LUM_LOG_WARN( "Cannot load image. {} is directory!", path.ToString( ) );
			return {};
		}

		auto normalizedPath = path.LexicallyNormal( );
		auto loweredPath = ToLower( normalizedPath.ToString( ) );

		auto hash = HashString( loweredPath );

		auto it = m_FilenameToHandle.find( hash );
		if (it != m_FilenameToHandle.end( )) {
			return it->second;
		}

		auto loadedImg = ResourceLoader::LoadImageFromFile( path );

		if (!loadedImg) {
			LUM_LOG_WARN( "Cannot load {} image: {}", path.ToString( ), loadedImg.GetError( ) );
			return {};
		}

		auto handle = m_Images.Append( loadedImg.ValueRef( ) );
		m_FilenameToHandle[ hash ] = handle;

		return handle;

	}
	ResourceImageHandle ResourceRegistry::GetImageHandle( const Path& path ) noexcept {

		auto normalizedPath = path.LexicallyNormal( );
		auto loweredPath = ToLower( normalizedPath.ToString( ) );

		auto hash = HashString( loweredPath );

		auto it = m_FilenameToHandle.find( hash );
		if (it != m_FilenameToHandle.end( )) {
			return it->second;
		}

		LUM_LOG_WARN( "ResourceImageHandle from file {} doesn't exist", normalizedPath.ToString( ) );
		return {};

	}
	Optional<const ImageData&> ResourceRegistry::GetCachedImage( const ResourceImageHandle handle ) const noexcept {
		if (m_Images.Contains( handle )) {
			return m_Images[ handle ];
		}
		return Optional<const ImageData&>::Empty();
	}

	void ResourceRegistry::RemoveCachedImage( ResourceImageHandle& handle ) noexcept {

		if (!handle)
			return;

		for (auto it = m_FilenameToHandle.begin( ); it != m_FilenameToHandle.end( ); ++it) {

			if (it->second == handle) {
				m_FilenameToHandle.erase( it );
				break;
			}
		}

		m_Images.Remove( handle );

	}

} // namespace lum