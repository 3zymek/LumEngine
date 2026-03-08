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

	void LightPass::Initialize( const FRendererContext& ctx ) {

		mContext = ctx;

		init();

	}

	void LightPass::AddPointLight( const FPointLight& light ) {

		LUM_ASSERT(mActivePointLights + 1 <= mPointLights.size(), "Max point lights reached");

		mPointLights[mActivePointLights++] = light;

	}
	void LightPass::AddSpotLight( const FSpotLight& light ) {

		LUM_ASSERT(mActiveSpotLights + 1 <= mSpotLights.size(), "Max spot lights reached");

		mSpotLights[mActiveSpotLights++] = light;

	}


	void LightPass::SetDirectionalLight( const FDirectionalLight& light ) {

		mDirectionalLightData.mColor = glm::vec4(light.mColor, 0.0);
		mDirectionalLightData.mDirection = glm::vec4(light.mDirection, 0.0);
		mDirectionalLightData.mIntensity = light.mIntensity;

	}

	FDirectionalLight LightPass::GetDirectionalLight( ) {
		return {
			glm::vec3(mDirectionalLightData.mDirection),
			mDirectionalLightData.mIntensity,
			glm::vec3(mDirectionalLightData.mColor)
		};
	}
	FDirectionalLight LightPass::GetDirectionalLight( ) const {
		return {
			glm::vec3(mDirectionalLightData.mDirection),
			mDirectionalLightData.mIntensity,
			glm::vec3(mDirectionalLightData.mColor)
		};
	}

	void LightPass::Execute( const ShadowPass& shadowPass, const detail::GBuffer& gbuffer, const detail::FScreenQuad& quad ) {

		upload_directional_light();
		upload_point_lights();
		upload_spot_lights();

		mContext.mRenderDevice->BindShader(mShader);
		mContext.mRenderDevice->BindTexture(shadowPass.GetShadowMap(), LUM_SHADOW_MAP);
		gbuffer.BindTextures();
		
		mContext.mRenderDevice->DrawElements(quad.mVao, 6);

	}




	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void LightPass::init( ) {

		rhi::FBufferDescriptor desc;
		desc.mBufferUsage = rhi::BufferUsage::Dynamic;
		desc.mMapFlags = rhi::MapFlag::Write;

		{ // Point Lights SSBO
			desc.mSize = (sizeof(FPointLight) * LUM_MAX_LIGHTS + sizeof(int32)) + (sizeof(FSpotLight) * LUM_MAX_LIGHTS + sizeof(int32));
			desc.mBufferType = rhi::BufferType::ShaderStorage;
			mLightsBuffer = mContext.mRenderDevice->CreateBuffer(desc);
			mContext.mRenderDevice->SetShaderStorageBinding(mLightsBuffer, LUM_SSBO_LIGHTS_BINDING);
		}
		{ // Directional Light UBO
			desc.mSize = sizeof(mDirectionalLightData);
			desc.mBufferType = rhi::BufferType::Uniform;
			mDirectionalLightBuffer = mContext.mRenderDevice->CreateBuffer(desc);
			mContext.mRenderDevice->SetUniformBufferBinding(mDirectionalLightBuffer, LUM_UBO_DIRECTIONAL_LIGHT);
		}
		
		mShader = mContext.mShaderMgr->LoadShader("shaders/light_pass.vert", "shaders/light_pass.frag", RootID::Internal);

	}

	void LightPass::upload_point_lights( ) {

		mContext.mRenderDevice->UpdateBuffer(
			mLightsBuffer, &mActivePointLights,
			OFFSET_ACTIVE_POINT, sizeof(int32)
		);

		mContext.mRenderDevice->UpdateBuffer(
			mLightsBuffer, mPointLights.data(),
			OFFSET_POINT_LIGHTS, sizeof(FPointLight) * LUM_MAX_LIGHTS
		);

	}
	void LightPass::upload_spot_lights( ) {

		mContext.mRenderDevice->UpdateBuffer(
			mLightsBuffer, &mActiveSpotLights,
			OFFSET_ACTIVE_SPOT, sizeof(int32)
		);

		mContext.mRenderDevice->UpdateBuffer(
			mLightsBuffer, mSpotLights.data(),
			OFFSET_SPOT_LIGHTS, sizeof(FSpotLight) * LUM_MAX_LIGHTS
		);

	}
	void LightPass::upload_directional_light( ) {
		
		mContext.mRenderDevice->UpdateBuffer(
			mDirectionalLightBuffer,
			&mDirectionalLightData
		);
	
	}

} // namespace lum::render