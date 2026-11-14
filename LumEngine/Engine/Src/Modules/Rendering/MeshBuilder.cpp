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
#include "Rendering/MeshBuilder.hpp"

#include "Core/Utils/ResourceRegistry.hpp"
#include "Rhi/Interfaces/Core/RhiBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiDevice.hpp"

namespace lum {

	void MeshBuilder::Initialize( SafePtr<RendererContext> ctx ) {

		ctx( ).Assert( );

		m_Ctx = ctx;

	}
	
	MeshRenderResources MeshBuilder::GetOrCreateMeshRenderResources( ResourceMeshHandle mesh ) noexcept {

		if (m_MeshToResources.contains( mesh )) {
			return m_MeshToResources[ mesh ];
		}

		auto meshOpt = ResourceRegistry::GetStatic().GetCachedMesh( mesh );
		if (!meshOpt) {
			return {};
		}

		const auto& meshData = meshOpt.Value( );

		MeshRenderResources renderResources{};

		{ // Vertex buffer

			rhi::BufferCreateInfo info{};
			info.m_BufferSize = meshData.m_Vertices.size( ) * sizeof( rhi::Vertex );
			info.m_Data = meshData.m_Vertices.data( );
			info.m_DataSize = meshData.m_Vertices.size( ) * sizeof( rhi::Vertex );
			info.m_Usage = rhi::BufferUsageFlag::Vertex;
			renderResources.m_VertexBuffer = m_Ctx( ).m_RenderDevice( ).CreateBuffer( info );
			renderResources.m_VertexBuffer = meshData.m_Vertices.size( );

		}
		{ // Element buffer

			rhi::BufferCreateInfo info{};
			info.m_BufferSize = meshData.m_Indices.size( ) * sizeof( uint32 );
			info.m_Data = meshData.m_Indices.data( );
			info.m_DataSize = meshData.m_Indices.size( ) * sizeof( uint32 );
			info.m_Usage = rhi::BufferUsageFlag::Element;
			renderResources.m_ElementBuffer = m_Ctx( ).m_RenderDevice( ).CreateBuffer( info );
			renderResources.m_IndicesCount = meshData.m_Indices.size( );

		}

		m_MeshToResources[ mesh ] = renderResources;

		return renderResources;

	}

} // namespace lum