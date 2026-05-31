//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Geometry render pass — handles per-object draw calls,
//          model matrix and material uniform uploads.
//
//=============================================================================//

#include "render/passes/geometry_pass.hpp"
#include "render/render_common.hpp"
#include "render/shader_manager.hpp"
#include "render/mesh_manager.hpp"
#include "render/deferred_buffer.hpp"

#include "rhi/core/rhi_device.hpp"

#include "entity/components/mesh.hpp"

namespace lum::render {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void GeometryPass::Initialize( const RendererContext& ctx ) {

		ValidateRendererContext( ctx );

		mContext = ctx;

		mInstances.reserve( limits::kMaxDrawCallsPerFrame );
		mTempInstances.reserve( mTempSize );

		init( );

	}

	void GeometryPass::Submit( const FRenderInstance& instance ) {

		LUM_ASSERT(
			mInstances.size( ) < limits::kMaxDrawCallsPerFrame,
			"Draw calls per frame limit reached"
		);

		mInstances.push_back( instance );

	}

	void GeometryPass::Execute( const detail::DeferredBuffer& gbuffer ) {

		gbuffer.BindBuffer( );
		mContext.mRenderDev->Clear(
			rhi::BufferBit::Color |
			rhi::BufferBit::Depth |
			rhi::BufferBit::Stencil
		);

		mContext.mRenderDev->BindShader( mShader );

		mContext.mRenderDev->BindPipeline( mPipeline );
		for (auto& instance : mInstances)
			draw_instance( instance );

		mInstances.clear( );

		gbuffer.UnbindBuffer( );

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
			desc.mSize = sizeof( detail::FModelUBOData );
			desc.mBufferType = rhi::BufferType::Uniform;
			mModelUniform = mContext.mRenderDev->CreateBuffer( desc );
			mContext.mRenderDev->SetUniformBufferBinding( mModelUniform, LUM_UBO_MODEL_BINDING );
		}
		{ // Material Uniform
			desc.mSize = sizeof( detail::FMaterialUBOData );
			desc.mBufferType = rhi::BufferType::Uniform;
			mMaterialUniform = mContext.mRenderDev->CreateBuffer( desc );
			mContext.mRenderDev->SetUniformBufferBinding( mMaterialUniform, LUM_UBO_MATERIAL_BINDING );
		}
		{ // Geometry pipeline
			rhi::PipelineCreateInfo desc;
			desc.mDepthStencil.mDepth.bEnabled = true;
			desc.mDepthStencil.mDepth.bWriteToZBuffer = true;
			desc.mDepthStencil.mDepth.mCompare = rhi::CompareFlag::Less;
			desc.mCull.bEnabled = true;
			desc.mCull.mFace = rhi::Face::Back;
			mPipeline = mContext.mRenderDev->CreatePipeline( desc );
			mShader = mContext.mShaderMgr->LoadShader( "shaders/geometry_pass.vert", "shaders/geometry_pass.frag", ResourceRoot::Internal );
		}

	}

	void GeometryPass::draw_instance( const FRenderInstance& instance ) {

		const auto* mat = instance.mMaterial;

		upload_material( *mat );

		mContext.mRenderDev->BindTexture( mat->mAlbedoTex, LUM_TEX_ALBEDO );
		mContext.mRenderDev->BindTexture( mat->mNormalTex, LUM_TEX_NORMAL );
		mContext.mRenderDev->BindTexture( mat->mRoughnessTex, LUM_TEX_ROUGHNESS );
		mContext.mRenderDev->BindTexture( mat->mMetallicTex, LUM_TEX_METALNESS );

		draw_mesh( instance );

	}

	void GeometryPass::draw_mesh( const FRenderInstance& instance ) {

		const FStaticMeshResource& res = mContext.mMeshMgr->GetStatic( instance.mStaticMesh->mHandle );

		upload_model_matrix( instance );
		mContext.mRenderDev->DrawElements( res.mVao, res.mNumIndices );

	}

	void GeometryPass::upload_model_matrix( const FRenderInstance& instance ) {

		mContext.mRenderDev->UpdateBuffer( mModelUniform, instance.mTransform->mWorldMatrix.Data(), 0, 0 );

	}
	void GeometryPass::upload_material( const CMaterialInstance& mat ) {

		mMaterialUBO.mBaseColor = Vector4( mat.mBaseColor, 1.0f );
		mMaterialUBO.mRoughness = mat.mRoughnessValue;
		mMaterialUBO.mMetallic = mat.mMetallicValue;

		mContext.mRenderDev->UpdateBuffer( mMaterialUniform, &mMaterialUBO );

	}


} // namespace lum::render