//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Light render pass — handles point and directional light
//          submission, uniform uploads and lighting pipeline management.
//
//=============================================================================//

#include "render/passes/light_pass.hpp"
#include "rhi/core/rhi_device.hpp"
#include "render/renderer.hpp"
#include "render/shader_manager.hpp"
#include "render/mesh_manager.hpp"

namespace lum::render {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void LightPass::Initialize(const FRendererContext& ctx) {

		mContext = ctx;

		init();

	}

	void LightPass::AddPointLight(const FPointLight& light) {

		LUM_ASSERT(mLights.mActivePoints + 1 <= mLights.mPoints.size(), "Max point lights reached");

		mPointLights[mActivePoints++] = light;

	}

	void LightPass::BeginPass() {

		clear_point_lights();

	}
	void LightPass::EndPass() {

		upload_directional_light();
		upload_point_lights();

	}





	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void LightPass::init() {

		rhi::FBufferDescriptor desc;
		desc.mBufferUsage = rhi::EBufferUsage::Dynamic;
		desc.mMapFlags = rhi::EMapFlag::Write;

		{ // Point Lights SSBO
			desc.mSize = sizeof(FPointLight) * LUM_MAX_LIGHTS + sizeof(int);
			desc.mBufferType = rhi::EBufferType::ShaderStorage;
			mPointLightsBuffer = mContext.mRenderDevice->CreateBuffer(desc);
			mContext.mRenderDevice->SetShaderStorageBinding(mPointLightsBuffer, LUM_SSBO_LIGHTS_BINDING);
		}
		{ // Directional Light UBO
			desc.mSize = sizeof(mDirectionalLightStruct);
			desc.mBufferType = rhi::EBufferType::Uniform;
			mDirectionalLightBuffer = mContext.mRenderDevice->CreateBuffer(desc);
			mContext.mRenderDevice->SetUniformBufferBinding(mDirectionalLightBuffer, LUM_UBO_DIRECTIONAL_LIGHT);
		}

	}
	void LightPass::upload_lightspace_matrix(const glm::mat4& mat) {

	}
	void LightPass::upload_point_lights() {

		mContext.mRenderDevice->UpdateBuffer(
			mPointLightsBuffer,
			mPointLights.data(),
			0,
			sizeof(FPointLight) * mActivePoints
		);

		mContext.mRenderDevice->UpdateBuffer(
			mPointLightsBuffer,
			&mActivePoints,
			sizeof(FPointLight) * LUM_MAX_LIGHTS,
			sizeof(mActivePoints)
		);
	}
	void LightPass::upload_directional_light() {
		
		mContext.mRenderDevice->UpdateBuffer(
			mDirectionalLightBuffer,
			&mDirectionalLightStruct
		);
	
	}

} // namespace lum::render