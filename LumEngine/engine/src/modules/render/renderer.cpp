//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Bridge between engine data and the RHI backend.
//
//=============================================================================//

#include "render/renderer.hpp"
#include "core/core_common.hpp"
#include "render/texture_manager.hpp"
#include "render/shader_manager.hpp"
#include "entity/components/transform.hpp"

namespace lum::render {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void Renderer::Initialize(const FRendererContext& ctx) {

		mContext = ctx;

		mGeometryPass.Initialize(mContext);
		mLightPass.Initialize(mContext);
		mEnvironmentPass.Initialize(mContext);
		mGBuffer.Initialize(mContext, 500, 500);

		init();

	}

	void Renderer::UpdateCamera(const FRenderCamera& camera) {

		mCameraStruct.mPosition = glm::vec4(camera.mPosition, 0.0);
		mCameraStruct.mProjection = camera.mProjection;
		mCameraStruct.mView = camera.mView;

		upload_camera_uniform();

	}

	void Renderer::BeginFrame() {

		mContext.mRenderDevice->BeginFrame();

		mGBuffer.BeginPass();
		mGeometryPass.BeginPass();
		mLightPass.BeginPass();
		mEnvironmentPass.BeginPass();

	}

	void Renderer::EndFrame() {
		
		mGeometryPass.EndPass();
		mGBuffer.EndPass();
		mLightPass.EndPass();
		mEnvironmentPass.EndPass();

		mContext.mRenderDevice->EndFrame();

	}





	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void Renderer::init() {
		rhi::FBufferDescriptor desc;
		desc.mBufferUsage = rhi::EBufferUsage::Dynamic;
		desc.mMapFlags = rhi::EMapFlag::Write;

		{ // Camera Uniform
			desc.mSize = sizeof(detail::FCameraUniformBuffer);
			desc.mBufferType = rhi::EBufferType::Uniform;
			mCameraBuffer = mContext.mRenderDevice->CreateBuffer(desc);
			mContext.mRenderDevice->SetUniformBufferBinding(mCameraBuffer, LUM_UBO_CAMERA_BINDING);
		}
	}

	void Renderer::upload_camera_uniform() {

		mContext.mRenderDevice->UpdateBuffer(mCameraBuffer, &mCameraStruct, 0, 0);

	}

}