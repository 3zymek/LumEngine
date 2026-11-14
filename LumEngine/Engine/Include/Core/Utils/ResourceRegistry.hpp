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
#pragma once

#include <unordered_map>

#include "Core/CoreTypes.hpp"
#include "Core/Limits.hpp"
#include "Core/Utils/HandlePool.hpp"
#include "Core/Utils/LogMacros.hpp"
#include "Core/Utils/Optional.hpp"
#include "Core/Utils/ResourceLoader.hpp"
#include "Core/Utils/StringUtils.hpp"
#include "Platform/FileSystem/Path.hpp"

namespace lum {

	enum class ResourceType : uint8 {
		Image = 0U,
		Mesh
	};

	namespace detail {
		struct ResourceImageTag;
		struct ResourceMeshTag;
	}

	using ResourceImageHandle = BaseHandle<detail::ResourceImageTag, uint32>;
	using ResourceMeshHandle = BaseHandle<detail::ResourceMeshTag, uint32>;

	namespace detail {

		template<ResourceType tType>
		struct ResourceHandleTraits;

		template<>
		struct ResourceHandleTraits<ResourceType::Image> {
			using Type = ResourceImageHandle;
		};

		template<>
		struct ResourceHandleTraits<ResourceType::Mesh> {
			using Type = ResourceMeshHandle;
		};

	}

	template<ResourceType tType>
	using ResourceHandleType = typename detail::ResourceHandleTraits<tType>::Type;

	class ResourceRegistry {
	public:

		static ResourceRegistry& GetStatic( ) noexcept {
			static ResourceRegistry inst{};
			return inst;
		}


		// Load resource

		template<ResourceType tResource>
		ResourceHandleType<tResource> LoadResourceFromFile( const Path& path ) noexcept {

			if (path.IsDirectory( )) {
				LUM_LOG_WARN( "Cannot load resource, {} is directory!", path.ToString( ) );
				return {};
			}

			auto normalizedPath = path.LexicallyNormal( );
			auto loweredPath = ToLower( normalizedPath.ToString( ) );
			auto hash = HashString( loweredPath );

			if constexpr (tResource == ResourceType::Image) {

				auto it = m_FilenameToImageHandle.find( hash );

				if (it != m_FilenameToImageHandle.end( ))
					return it->second;

				auto loadedImage = ResourceLoader::LoadImageFromFile( normalizedPath );

				if (!loadedImage) {
					LUM_LOG_WARN( "Cannot load image {}: {}", normalizedPath.ToString( ), loadedImage.GetError( ) );
					return {};
				}

				auto handle = m_Images.Append( loadedImage.ValueRef( ) );

				m_FilenameToImageHandle.emplace( hash, handle );

				return handle;

			}
			else if constexpr (tResource == ResourceType::Mesh) {

				auto it = m_FilenameToMeshHandle.find( hash );

				if (it != m_FilenameToMeshHandle.end( ))
					return it->second;

				auto loadedMesh = ResourceLoader::LoadMeshFromFile( normalizedPath );

				if (!loadedMesh) {
					LUM_LOG_WARN( "Cannot load mesh {}: {}", normalizedPath.ToString( ), loadedMesh.GetError( ) );
					return {};
				}

				auto handle = m_Meshes.Append( loadedMesh.ValueRef( ) );

				m_FilenameToMeshHandle.emplace( hash, handle );

				return handle;

			}

		}


		// Get handle

		template<ResourceType tResource>
		ResourceHandleType<tResource> GetHandle( const Path& path ) const noexcept {

			auto normalizedPath = path.LexicallyNormal( );
			auto loweredPath = ToLower( normalizedPath.ToString( ) );
			auto hash = HashString( loweredPath );

			if constexpr (tResource == ResourceType::Image) {

				auto it = m_FilenameToImageHandle.find( hash );

				if (it != m_FilenameToImageHandle.end( ))
					return it->second;

			}
			else if constexpr (tResource == ResourceType::Mesh) {

				auto it = m_FilenameToMeshHandle.find( hash );

				if (it != m_FilenameToMeshHandle.end( ))
					return it->second;

			}

			LUM_LOG_WARN( "Resource handle from file {} doesn't exist!", normalizedPath.ToString( ) );

			return {};
		}


		// Remove cached resource

		template<ResourceType tResource>
		void RemoveCachedResource( ResourceHandleType<tResource>& handle ) noexcept {

			if (!handle)
				return;

			if constexpr (tResource == ResourceType::Image) {

				for (auto it = m_FilenameToImageHandle.begin( );
					 it != m_FilenameToImageHandle.end( ); ++it) {

					if (it->second == handle) {
						m_FilenameToImageHandle.erase( it );
						break;
					}
				}

				m_Images.Remove( handle );

			}
			else if constexpr (tResource == ResourceType::Mesh) {

				for (auto it = m_FilenameToMeshHandle.begin( );
					 it != m_FilenameToMeshHandle.end( ); ++it) {

					if (it->second == handle) {
						m_FilenameToMeshHandle.erase( it );
						break;
					}
				}

				m_Meshes.Remove( handle );

			}

			handle = {};
		}


		// Get cached resource data

		Optional<const ImageData&> GetCachedImage( const ResourceImageHandle handle ) const noexcept;
		Optional<const MeshGeometry&> GetCachedMesh( const ResourceMeshHandle handle ) const noexcept;


	private:

		HandlePool<ResourceImageHandle, ImageData> m_Images{ limits::k_MaxLoadedImages };
		HandlePool<ResourceMeshHandle, MeshGeometry> m_Meshes{ limits::k_MaxLoadedMeshes };

		std::unordered_map<HashedString, ResourceImageHandle> m_FilenameToImageHandle{};
		std::unordered_map<HashedString, ResourceMeshHandle> m_FilenameToMeshHandle{};

		ResourceRegistry( ) = default;
		ResourceRegistry( const ResourceRegistry& ) = delete;
		ResourceRegistry( ResourceRegistry&& ) = delete;
		ResourceRegistry& operator=( const ResourceRegistry& ) = delete;
		ResourceRegistry& operator=( ResourceRegistry&& ) = delete;

	};

} // namespace lum