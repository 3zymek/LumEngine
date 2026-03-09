//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Geometry render pass — handles per-object draw calls,
//          model matrix and material uniform uploads.
//
//=============================================================================//

#include "render/passes/geometry_pass.hpp"
#include "render/common.hpp"

#include "rhi/core/rhi_device.hpp"

#include "render/renderer.hpp"
#include "render/shader_manager.hpp"
#include "render/mesh_manager.hpp"

#include "render/g_buffer.hpp"

namespace lum::render {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void GeometryPass::Initialize( const FRendererContext& ctx ) {

		mContext = ctx;

		mInstances.reserve(limits::gMaxDrawCallsPf);
		mTempInstances.reserve(mTempSize);

		init();

	}

	void GeometryPass::Submit( const FRenderInstance& instance ) {
	
		LUM_HOTCHK_RETURN_VOID(
			(mInstances.size() < limits::gMaxDrawCallsPf), 
			LUM_SEV_WARN, 
			"Draw calls per frame limit reached"
		);

		mInstances.push_back(instance);

	}

	void GeometryPass::Execute( const detail::GBuffer& gbuffer ) {
		
		gbuffer.BindBuffer();
		mContext.mRenderDevice->Clear(
			rhi::BufferBit::Color |
			rhi::BufferBit::Depth |
			rhi::BufferBit::Stencil
		);

		mContext.mRenderDevice->BindShader(mShader);

		mContext.mRenderDevice->BindPipeline(mPipeline);
		for (auto& instance : mInstances)
			draw_instance(instance);

		mInstances.clear();

		gbuffer.UnbindBuffer();

	}

	void GeometryPass::DrawScene( ) {

		for (auto& instance : mInstances)
			draw_instance(instance);

	}





	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void GeometryPass::init( ) {

		rhi::FBufferDescriptor desc;
		desc.mBufferUsage = rhi::BufferUsage::Dynamic;
		desc.mMapFlags = rhi::MapFlag::Write;
		{ // Model Uniform
			desc.mSize = sizeof(detail::FModelUniformBuffer);
			desc.mBufferType = rhi::BufferType::Uniform;
			mModelUniform = mContext.mRenderDevice->CreateBuffer(desc);
			mContext.mRenderDevice->SetUniformBufferBinding(mModelUniform, LUM_UBO_MODEL_BINDING);
		}
		{ // Material Uniform
			desc.mSize = sizeof(detail::FMaterialUniformBuffer);
			desc.mBufferType = rhi::BufferType::Uniform;
			mMaterialUniform = mContext.mRenderDevice->CreateBuffer(desc);
			mContext.mRenderDevice->SetUniformBufferBinding(mMaterialUniform, LUM_UBO_MATERIAL_BINDING);
		}
		{ // Geometry pipeline
			rhi::FPipelineDescriptor desc;
			desc.mDepthStencil.mDepth.bEnabled = true;
			desc.mDepthStencil.mDepth.bWriteToZBuffer = true;
			desc.mDepthStencil.mDepth.mCompare = rhi::CompareFlag::Less;
			desc.mCull.bEnabled = true;
			desc.mCull.mFace = rhi::Face::Back;
			mPipeline = mContext.mRenderDevice->CreatePipeline(desc);
			mShader = mContext.mShaderMgr->LoadShader("shaders/geometry_pass.vert", "shaders/geometry_pass.frag", RootID::Internal);
		}
		
	}

	void GeometryPass::draw_instance( const FRenderInstance& instance ) {
		
		const auto& mat = instance.mMaterial->mMat;

		upload_material(mat);

		mContext.mRenderDevice->BindTexture(mat.mAlbedoTex, LUM_TEX_ALBEDO);
		mContext.mRenderDevice->BindTexture(mat.mNormalTex, LUM_TEX_NORMAL);
		mContext.mRenderDevice->BindTexture(mat.mRoughnessTex, LUM_TEX_ROUGHNESS);
		mContext.mRenderDevice->BindTexture(mat.mMetallicTex, LUM_TEX_METALNESS);
		
		draw_mesh(instance);

	}

	void GeometryPass::draw_mesh( const FRenderInstance& instance ) {

		const FStaticMeshResource& res = mContext.mMeshMgr->GetStatic(instance.mStaticMesh->mMesh);

		upload_model_matrix(instance);
		mContext.mRenderDevice->DrawElements(res.mVao, res.mNumIndices);

	}

	void GeometryPass::upload_model_matrix( const FRenderInstance& instance ) {

		glm::quat rot = glm::quat(glm::radians(instance.mTransform->mRotation));
		glm::mat4 rotation = glm::mat4_cast(rot);
		glm::mat4 model = glm::mat4(1.f);

		model = glm::mat4(1.f);
		model = glm::translate(model, instance.mTransform->mPosition);
		model = model * rotation;
		model = glm::scale(model, instance.mTransform->mScale);

		mContext.mRenderDevice->UpdateBuffer(mModelUniform, glm::value_ptr(model), 0, 0);

	}
	void GeometryPass::upload_material( const FMaterialInstance& mat ) {

		mMaterialUBO.mBaseColor = glm::vec4(mat.mBaseColor, 1.0);
		mMaterialUBO.mRoughness = mat.mRoughnessValue;
		mMaterialUBO.mMetallic = mat.mMetallicValue;

		mContext.mRenderDevice->UpdateBuffer(mMaterialUniform, &mMaterialUBO);

	}


} // namespace lum::render