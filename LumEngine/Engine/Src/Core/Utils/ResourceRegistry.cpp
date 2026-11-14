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
#include "Core/Utils/HandlePool.hpp"
#include "Core/Utils/Optional.hpp"

namespace lum {

	Optional<const ImageData&> ResourceRegistry::GetCachedImage( const ResourceImageHandle handle ) const noexcept {
		if (m_Images.Contains( handle )) {
			return m_Images[ handle ];
		}
		return Optional<const ImageData&>::Empty();
	}

	Optional<const MeshGeometry&> ResourceRegistry::GetCachedMesh( const ResourceMeshHandle handle ) const noexcept {
		if (m_Meshes.Contains( handle )) {
			return m_Meshes[ handle ];
		}
		return Optional<const MeshGeometry&>::Empty( );
	}

} // namespace lum