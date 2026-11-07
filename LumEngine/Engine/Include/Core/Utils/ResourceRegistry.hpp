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
#include "Core/Utils/Optional.hpp"
#include "Core/Utils/ResourceLoader.hpp"

namespace lum {
	
	class Path;

	namespace detail {
		struct ResourceImageTag;
	} // namespace detail

	using ResourceImageHandle = BaseHandle<detail::ResourceImageTag, uint32>;

	class ResourceRegistry {
	public:

		static ResourceRegistry& GetStatic( ) noexcept {
			static ResourceRegistry inst{};
			return inst;
		}

		ResourceImageHandle LoadImageFromFile( const Path& path ) noexcept;
		ResourceImageHandle GetImageHandle( const Path& path ) noexcept;
		Optional<const ImageData&> GetCachedImage( const ResourceImageHandle handle ) const noexcept;
		void RemoveCachedImage( ResourceImageHandle& handle ) noexcept;

	private:

		std::unordered_map<HashedString, ResourceImageHandle> m_FilenameToHandle{};
		HandlePool<ResourceImageHandle, ImageData> m_Images{ limits::k_MaxLoadedImagesCapacity };

		ResourceRegistry( ResourceRegistry&& ) = delete;
		ResourceRegistry( const ResourceRegistry&& ) = delete;
		ResourceRegistry( ResourceRegistry& ) = delete;
		ResourceRegistry( const ResourceRegistry& ) = delete;
		ResourceRegistry( ) = default;


	};

} // namespace lum