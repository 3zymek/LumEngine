//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Loads and manages static mesh resources using RHI.
//
//=============================================================================//

#include "Render/MeshManager.hpp"
#include "Core/Utils/ResourceLoader.hpp"
#include "Render/Mesh.hpp"
#include "Event/EventBus.hpp"
#include "Event/Events/EntityEvents.hpp"
#include "Entity/Components/Mesh.hpp"

namespace lum {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void MeshManager::Initialize( render::RendererContext& ctx ) {

		ctx.Validate( );

		m_Ctx = &ctx;
		init( );

	}

	const StaticMeshResource& MeshManager::GetStatic( StaticMeshHandle handle ) {
		if (m_StaticMeshes.Contains( handle ))
			return m_StaticMeshes[ handle ];
		else
			return m_StaticMeshes[ m_DefaultMesh ];
	}

	StaticMeshHandle MeshManager::CreateStatic( StringView path, ResourceRoot root ) {

		uint64 hash = HashString( path );

		if (m_StaticMeshCache.contains( hash ))
			return m_StaticMeshCache[ hash ];

		auto meshGeometry = ResourceLoader::LoadMeshFromFile( root, path );

		if (!meshGeometry) {
			LUM_LOG_ERROR( "Failed to load model %s: %s", path.data(), meshGeometry.GetError() );
			return m_ErrorMesh;
		}

		detail::RenderResources res = upload_gpu( detail::MeshType::Static, meshGeometry.ValueRef( ) );

		StaticMeshResource meshResource;
		meshResource.m_Vbo = res.m_Vbo;
		meshResource.m_Ebo = res.m_Ebo;
		meshResource.m_Vao = res.m_Vao;
		meshResource.m_NumIndices = meshGeometry.ValueRef( ).m_Indices.size( );

		StaticMeshHandle meshHandle = m_StaticMeshes.Append( std::move( meshResource ) );

		m_StaticMeshCache[ hash ] = meshHandle;

		return meshHandle;
	}

	DynamicMeshInstance MeshManager::CreateDynamic( StringView path, ResourceRoot root ) {

		auto meshGeometry = ResourceLoader::LoadMeshFromFile( root, path );

		if (!meshGeometry) {
			LUM_LOG_ERROR( "Failed to load model %s: %s", path, meshGeometry.GetError() );
			MeshGeometry fallback;
			fallback.m_Vertices = m_DefaultVertices;
			fallback.m_Indices = m_DefaultIndices;
			meshGeometry = fallback;
		}

		detail::RenderResources res = upload_gpu( detail::MeshType::Dynamic, meshGeometry.ValueRef( ) );

		DynamicMeshInstance meshInstance;
		meshInstance.m_Data = meshGeometry.ValueRef( );
		meshInstance.m_Vbo = res.m_Vbo;
		meshInstance.m_Ebo = res.m_Ebo;
		meshInstance.m_Vao = res.m_Vao;

		return meshInstance;
	}




	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void MeshManager::init( ) {

		m_Ctx( ).m_EventBus( ).SubscribePermanently<EComponentAdded<CStaticMesh>>(
			[&]( const EComponentAdded<CStaticMesh>& mesh ) {
				
				if(!mesh.m_Component->m_Path.empty())
					mesh.m_Component->m_Handle = CreateStatic( mesh.m_Component->m_Path );

			}
		);

		create_meshes( );

	}

	detail::RenderResources MeshManager::upload_gpu( detail::MeshType type, const MeshGeometry& data ) {

		Flags<rhi::MapFlag> mapFlag{};
		rhi::BufferUsage usage{};

		if (type == detail::MeshType::Static) {

			mapFlag = rhi::MapFlag::None;
			usage = rhi::BufferUsage::Static;

		}
		else if (type == detail::MeshType::Dynamic) {

			mapFlag = rhi::MapFlag::Read | rhi::MapFlag::Write;
			usage = rhi::BufferUsage::Dynamic;

		}

		detail::RenderResources res;

		rhi::BufferCreateInfo vboDesc;
		vboDesc.m_BufferUsage = usage;
		vboDesc.m_Data = data.m_Vertices.data( );
		vboDesc.m_MapFlags = mapFlag;
		vboDesc.m_Size = ComputeByteSize( data.m_Vertices );
		vboDesc.m_BufferType = rhi::BufferType::Vertex;
		res.m_Vbo = m_Ctx().m_RenderDev().CreateBuffer( vboDesc );

		rhi::BufferCreateInfo eboDesc;
		eboDesc.m_BufferUsage = usage;
		eboDesc.m_Data = data.m_Indices.data( );
		eboDesc.m_MapFlags = mapFlag;
		eboDesc.m_Size = ComputeByteSize( data.m_Indices );
		eboDesc.m_BufferType = rhi::BufferType::Element;
		res.m_Ebo = m_Ctx().m_RenderDev().CreateBuffer( eboDesc );

		rhi::VertexAttribute vaoAttrib[ 5 ];

		auto& position = vaoAttrib[ 0 ];
		position.m_Format = rhi::DataFormat::Vec3;
		position.m_RelativeOffset = offsetof( Vertex, m_Position );
		position.m_ShaderLocation = LUM_LAYOUT_POSITION;

		auto& normal = vaoAttrib[ 1 ];
		normal.m_Format = rhi::DataFormat::Vec3;
		normal.m_RelativeOffset = offsetof( Vertex, m_Normal );
		normal.m_ShaderLocation = LUM_LAYOUT_NORMAL;

		auto& uv = vaoAttrib[ 2 ];
		uv.m_Format = rhi::DataFormat::Vec2;
		uv.m_RelativeOffset = offsetof( Vertex, m_Uv );
		uv.m_ShaderLocation = LUM_LAYOUT_UV;

		auto& tg = vaoAttrib[ 3 ];
		tg.m_Format = rhi::DataFormat::Vec3;
		tg.m_RelativeOffset = offsetof( Vertex, m_Tangent );
		tg.m_ShaderLocation = LUM_LAYOUT_TANGENT;

		auto& btg = vaoAttrib[ 4 ];
		btg.m_Format = rhi::DataFormat::Vec3;
		btg.m_RelativeOffset = offsetof( Vertex, m_Bitangent );
		btg.m_ShaderLocation = LUM_LAYOUT_BITANGENT;

		rhi::VertexLayoutCreateInfo vaoDesc;
		vaoDesc.m_Attributes = vaoAttrib;
		vaoDesc.m_Stride = sizeof( Vertex );
		res.m_Vao = m_Ctx( ).m_RenderDev( ).CreateVertexLayout( vaoDesc, res.m_Vbo );

		m_Ctx( ).m_RenderDev( ).AttachElementBufferToLayout( res.m_Ebo, res.m_Vao );

		return res;
	}

	void MeshManager::create_meshes( ) {
		{ // Default mesh

			MeshGeometry data;
			data.m_Vertices = m_DefaultVertices;
			data.m_Indices = m_DefaultIndices;

			detail::RenderResources res = upload_gpu( detail::MeshType::Static, data );

			StaticMeshResource staticMesh;
			staticMesh.m_Vbo = res.m_Vbo;
			staticMesh.m_Ebo = res.m_Ebo;
			staticMesh.m_Vao = res.m_Vao;
			staticMesh.m_NumIndices = data.m_Indices.size( );

			m_DefaultMesh = m_StaticMeshes.Append( std::move( staticMesh ) );

		}
		{ // Error mesh
			auto meshGeometry = ResourceLoader::LoadMeshFromFile( ResourceRoot::Internal, "models/ERRORText.fbx" );
			if (!meshGeometry) {
				LUM_LOG_ERROR( "Failed to load fallback error model: %s", meshGeometry.GetError() );
				m_ErrorMesh = m_DefaultMesh;
				return;
			}
			detail::RenderResources res = upload_gpu( detail::MeshType::Static, meshGeometry.ValueRef( ) );

			StaticMeshResource staticMesh;
			staticMesh.m_Vbo = res.m_Vbo;
			staticMesh.m_Ebo = res.m_Ebo;
			staticMesh.m_Vao = res.m_Vao;
			staticMesh.m_NumIndices = meshGeometry.ValueRef( ).m_Indices.size( );

			m_ErrorMesh = m_StaticMeshes.Append( std::move( staticMesh ) );

		}

	}

} // namespace lum