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
#include "Core/Utils/ResourceRegistry.hpp"
#include "Rhi/Interfaces/Core/RhiBuffer.hpp"
#include "Rendering/RenderingTypes.hpp"
#include "Core/Utils/SafePtr.hpp"

namespace lum {

	struct MeshRenderResources {
		rhi::BufferHandle m_VertexBuffer{};
		rhi::BufferHandle m_ElementBuffer{};
		uint32 m_IndicesCount = 0;
		uint32 m_VerticesCount = 0;
	};

}

namespace std {

	template<>
	struct hash<lum::ResourceMeshHandle> {
		size_t operator()( const lum::ResourceMeshHandle& handle ) const noexcept {
			size_t seed = std::hash<uint32_t>{}(handle.m_Id);
			const size_t generationHash = std::hash<uint32_t>{}(handle.m_Generation);

			seed ^= generationHash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			return seed;
		}
	};

} // namespace std

namespace lum {

	class MeshBuilder {
	public:

		void Initialize( SafePtr<RendererContext> ctx );

		MeshRenderResources GetOrCreateMeshRenderResources( ResourceMeshHandle handle ) noexcept;

	private:

		SafePtr<RendererContext> m_Ctx = nullptr;

		std::unordered_map<ResourceMeshHandle, MeshRenderResources> m_MeshToResources{};

	};

} // namespace lum