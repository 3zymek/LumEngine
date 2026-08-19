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

		mCtx = ctx;

		mInstances.reserve( limits::kMaxDrawCallsPerFrame );
		mTempInstances.reserve( mTempSize );

		init( );

	}

	void GeometryPass::Submit( const RenderInstance& instance ) {

		LUM_ASSERT(
			mInstances.size( ) < limits::kMaxDrawCallsPerFrame,
			"Draw calls per frame limit reached"
		);

		mInstances.push_back( instance );

	}

	void GeometryPass::Execute( const detail::DeferredBuffer& defferedBuff ) {

		mCtx( ).mRenderDev( ).BindShader( mShader );

		mCtx( ).mRenderDev( ).BindPipeline( mPipeline );
		for (auto& instance : mInstances)
			draw_instance( instance );

		mInstances.clear( );

		defferedBuff.UnbindFramebuffer( );

	}

	void GeometryPass::DrawScene( ) {

		for (auto& instance : mInstances)
			draw_instance( instance );

	}





	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void GeometryPass::init( ) {

		rhi::BufferCreateInfo desc;
		desc.mBufferUsage = rhi::BufferUsage::Dynamic;
		desc.mMapFlags = rhi::MapFlag::Write;
		{ // Model Uniform
			desc.mSize = sizeof( detail::ModelGPU );
			desc.mBufferType = rhi::BufferType::Uniform;
			mModelUniform = mCtx( ).mRenderDev( ).CreateBuffer( desc );
			mCtx( ).mRenderDev( ).SetUniformBufferBinding( mModelUniform, LUM_UBO_MODEL_BINDING );
		}
		{ // Material Uniform
			desc.mSize = sizeof( detail::MaterialGPU );
			desc.mBufferType = rhi::BufferType::Uniform;
			mMaterialUniform = mCtx( ).mRenderDev( ).CreateBuffer( desc );
			mCtx( ).mRenderDev( ).SetUniformBufferBinding( mMaterialUniform, LUM_UBO_MATERIAL_BINDING );
		}
		{ // Geometry pipeline
			rhi::PipelineCreateInfo desc;
			desc.mDepthStencil.mDepth.bEnabled = true;
			desc.mDepthStencil.mDepth.bWriteToZBuffer = true;
			desc.mDepthStencil.mDepth.mCompare = rhi::CompareFlag::Less;
			desc.mCull.bEnabled = true;
			desc.mCull.mFace = rhi::Face::Back;
			mPipeline = mCtx( ).mRenderDev( ).CreatePipeline( desc );
			mShader = mCtx( ).mShaderMgr( ).LoadShader( "shaders/geometry_pass.vert", "shaders/geometry_pass.frag", ResourceRoot::Internal );
		}

	}

	void GeometryPass::draw_instance( const RenderInstance& instance ) {

		const auto* mat = instance.mMaterial;

		upload_material( *mat );

		mCtx( ).mRenderDev( ).BindTexture( mat->mAlbedoTex, LUM_TEX_ALBEDO );
		mCtx( ).mRenderDev( ).BindTexture( mat->mNormalTex, LUM_TEX_NORMAL );
		mCtx( ).mRenderDev( ).BindTexture( mat->mRoughnessTex, LUM_TEX_ROUGHNESS );
		mCtx( ).mRenderDev( ).BindTexture( mat->mMetallicTex, LUM_TEX_METALNESS );

		draw_mesh( instance );

	}

	void GeometryPass::draw_mesh( const RenderInstance& instance ) {

		const StaticMeshResource& res = mCtx( ).mMeshMgr( ).GetStatic( instance.mStaticMesh->mHandle );

		upload_model_matrix( instance );
		mCtx( ).mRenderDev( ).DrawElements( res.mVao, res.mNumIndices );

	}

	void GeometryPass::upload_model_matrix( const RenderInstance& instance ) {

		mCtx( ).mRenderDev( ).UpdateBuffer( mModelUniform, instance.mTransform->mWorldMatrix.Data(), 0, 0 );

	}
	void GeometryPass::upload_material( const CMaterialInstance& mat ) {

		mMaterialUbo.mBaseColor = Vector4( mat.mBaseColor, 1.0f );
		mMaterialUbo.mRoughness = mat.mRoughnessValue;
		mMaterialUbo.mMetallic = mat.mMetallicValue;

		mCtx( ).mRenderDev( ).UpdateBuffer( mMaterialUniform, &mMaterialUbo );

	}


} // namespace lum::render