//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Geometry render pass — handles per-object draw calls,
//          model matrix and material uniform uploads.
//
//=============================================================================//

#include "Render/Passes/GeometryPass.hpp"
#include "Render/RenderCommon.hpp"
#include "Render/ShaderManager.hpp"
#include "Render/MeshManager.hpp"
#include "Render/DeferredBuffer.hpp"

#include "Rhi/Core/RhiDevice.hpp"

#include "Entity/Components/Mesh.hpp"
#include "Entity/Components/Transform.hpp"

namespace lum::render {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void GeometryPass::Initialize( RendererContext& ctx ) {

		ctx.Validate( );

		m_Ctx = ctx;

		m_Instances.reserve( limits::k_MaxDrawCallsPerFrame );
		m_TempInstances.reserve( m_TempSize );

		init( );

	}

	void GeometryPass::Submit( const RenderInstance& instance ) {

		LUM_ASSERT(
			m_Instances.size( ) < limits::k_MaxDrawCallsPerFrame,
			"Draw calls per frame limit reached"
		);

		m_Instances.push_back( instance );

	}

	void GeometryPass::Execute( const detail::DeferredBuffer& defferedBuff ) {

		m_Ctx( ).m_RenderDev( ).BindShader( m_Shader );

		m_Ctx( ).m_RenderDev( ).BindPipeline( m_Pipeline );
		for (auto& instance : m_Instances)
			draw_instance( instance );

		m_Instances.clear( );

		defferedBuff.UnbindFramebuffer( );

	}

	void GeometryPass::DrawScene( ) {

		for (auto& instance : m_Instances)
			draw_instance( instance );

	}





	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void GeometryPass::init( ) {

		rhi::BufferCreateInfo desc;
		desc.m_BufferUsage = rhi::BufferUsage::Dynamic;
		desc.m_MapFlags = rhi::MapFlag::Write;
		{ // Model Uniform
			desc.m_Size = sizeof( detail::ModelGPU );
			desc.m_BufferType = rhi::BufferType::Uniform;
			m_ModelUniform = m_Ctx( ).m_RenderDev( ).CreateBuffer( desc );
			m_Ctx( ).m_RenderDev( ).SetUniformBufferBinding( m_ModelUniform, LUM_UBO_MODEL_BINDING );
		}
		{ // Material Uniform
			desc.m_Size = sizeof( detail::MaterialGPU );
			desc.m_BufferType = rhi::BufferType::Uniform;
			m_MaterialUniform = m_Ctx( ).m_RenderDev( ).CreateBuffer( desc );
			m_Ctx( ).m_RenderDev( ).SetUniformBufferBinding( m_MaterialUniform, LUM_UBO_MATERIAL_BINDING );
		}
		{ // Geometry pipeline
			rhi::PipelineCreateInfo desc;
			desc.m_DepthStencil.m_Depth.bEnabled = true;
			desc.m_DepthStencil.m_Depth.bWriteToZBuffer = true;
			desc.m_DepthStencil.m_Depth.m_Compare = rhi::CompareFlag::Less;
			desc.m_Cull.bEnabled = true;
			desc.m_Cull.m_Face = rhi::Face::Back;
			m_Pipeline = m_Ctx( ).m_RenderDev( ).CreatePipeline( desc );
			m_Shader = m_Ctx( ).m_ShaderMgr( ).LoadShader( "shaders/geometry_pass.vert", "shaders/geometry_pass.frag", ResourceRoot::Internal );
		}

	}

	void GeometryPass::draw_instance( const RenderInstance& instance ) {

		const auto* mat = instance.m_Material;

		upload_material( *mat );

		m_Ctx( ).m_RenderDev( ).BindTexture( mat->m_AlbedoTex, LUM_TEX_ALBEDO );
		m_Ctx( ).m_RenderDev( ).BindTexture( mat->m_NormalTex, LUM_TEX_NORMAL );
		m_Ctx( ).m_RenderDev( ).BindTexture( mat->m_RoughnessTex, LUM_TEX_ROUGHNESS );
		m_Ctx( ).m_RenderDev( ).BindTexture( mat->m_MetallicTex, LUM_TEX_METALNESS );

		draw_mesh( instance );

	}

	void GeometryPass::draw_mesh( const RenderInstance& instance ) {

		const StaticMeshResource& res = m_Ctx( ).m_MeshMgr( ).GetStatic( instance.m_StaticMesh->m_Handle );

		upload_model_matrix( instance );
		m_Ctx( ).m_RenderDev( ).DrawElements( res.m_Vao, res.m_NumIndices );

	}

	void GeometryPass::upload_model_matrix( const RenderInstance& instance ) {

		m_Ctx( ).m_RenderDev( ).UpdateBuffer( m_ModelUniform, instance.m_Transform->m_WorldMatrix.Data(), 0, 0 );

	}
	void GeometryPass::upload_material( const CMaterialInstance& mat ) {

		m_MaterialUbo.m_BaseColor = Vector4( mat.m_BaseColor, 1.0f );
		m_MaterialUbo.m_Roughness = mat.m_RoughnessValue;
		m_MaterialUbo.m_Metallic = mat.m_MetallicValue;

		m_Ctx( ).m_RenderDev( ).UpdateBuffer( m_MaterialUniform, &m_MaterialUbo );

	}


} // namespace lum::render