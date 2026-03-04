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

		mGeometryPass.		Initialize(mContext);
		mLightPass.			Initialize(mContext);
		mEnvironmentPass.	Initialize(mContext);
		mGBuffer.			Initialize(mContext, 500, 500);

		init();

	}

	void Renderer::UpdateCamera(const FRenderCamera& camera) {

		mCameraStruct.mPosition = glm::vec4(camera.mPosition, 0.0);
		mCameraStruct.mProjection = camera.mProjection;
		mCameraStruct.mView = camera.mView;
		mCameraStruct.mInvViewProj = glm::inverse(camera.mView * camera.mProjection);

		upload_camera_uniform();

	}

	void Renderer::BeginFrame() {

		mContext.mRenderDevice->Clear(rhi::ClearFlag::Color | rhi::ClearFlag::Depth | rhi::ClearFlag::Stencil);

		mContext.mRenderDevice->NewFrame();

		mGBuffer.BeginPass();
		mGeometryPass.Execute();
		mGBuffer.EndPass();

		mLightPass.BeginPass();
		mEnvironmentPass.BeginPass();

	}

	void Renderer::EndFrame() {

		mLightPass.EndPass();

		mContext.mRenderDevice->SwapBuffers();

	}





	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void Renderer::init() {

		mContext.mEventBus->SubscribePermanently<EWindowResized>([this](const EWindowResized& e) {
			this->mContext.mRenderDevice->SetViewport(0, 0, e.mWidth, e.mHeight);
		});

		rhi::FBufferDescriptor desc;
		desc.mBufferUsage = rhi::BufferUsage::Dynamic;
		desc.mMapFlags = rhi::MapFlag::Write;

		{ // Camera Uniform
			desc.mSize = sizeof(detail::FCameraUniformBuffer);
			desc.mBufferType = rhi::BufferType::Uniform;
			mCameraBuffer = mContext.mRenderDevice->CreateBuffer(desc);
			mContext.mRenderDevice->SetUniformBufferBinding(mCameraBuffer, LUM_UBO_CAMERA_BINDING);
		}
	}

	void Renderer::upload_camera_uniform() {

		mContext.mRenderDevice->UpdateBuffer(mCameraBuffer, &mCameraStruct, 0, 0);

	}

}