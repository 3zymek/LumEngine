//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Light render pass — handles point and directional light
//          submission, uniform uploads and lighting pipeline management.
//
//=============================================================================//

#include "rhi/core/rhi_device.hpp"
#include "render/renderer.hpp"
#include "render/shader_manager.hpp"

namespace lum::render {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void LightPass::Initialize( const RendererContext& ctx ) {

		ValidateRendererContext( ctx );

		mContext = ctx;

		init( );

	}

	void LightPass::AddPointLight( const PointLight& light ) {

		LUM_ASSERT( mActivePointLights + 1 <= mPointLights.size( ), "Max point lights reached" );

		mPointLights[ mActivePointLights++ ] = light;

	}
	void LightPass::AddSpotLight( const SpotLight& light ) {

		LUM_ASSERT( mActiveSpotLights + 1 <= mSpotLights.size( ), "Max spot lights reached" );

		mSpotLights[ mActiveSpotLights++ ] = light;

	}


	void LightPass::SetDirectionalLight( const DirectionalLight& light ) {

		mDirectionalLightData.mColor = Vector4( light.mColor, 0.0f );
		mDirectionalLightData.mDirection = Vector4( light.mDirection, 0.0f );
		mDirectionalLightData.mIntensity = light.mIntensity;

	}

	DirectionalLight LightPass::GetDirectionalLight( ) {
		return {
			Vector3( mDirectionalLightData.mDirection ),
			mDirectionalLightData.mIntensity,
			Vector3( mDirectionalLightData.mColor )
		};
	}
	DirectionalLight LightPass::GetDirectionalLight( ) const {
		return {
			Vector3( mDirectionalLightData.mDirection ),
			mDirectionalLightData.mIntensity,
			Vector3( mDirectionalLightData.mColor )
		};
	}

	void LightPass::Execute( const detail::DeferredBuffer& gbuffer, const detail::ScreenQuad& quad, const FLightPassExecute& desc ) {

		mContext.mRenderDev->BindFramebuffer( quad.mSceneFbo );
		mContext.mRenderDev->BindPipeline( mPipeline );

		upload_directional_light( );
		upload_point_lights( );
		upload_spot_lights( );

		mContext.mRenderDev->BindShader( mShader );
		mContext.mRenderDev->BindTexture( desc.mShadowMap, LUM_SHADOW_MAP );
		mContext.mRenderDev->BindTexture( desc.mIrradianceMap, LUM_TEX_IRRADIANCE );
		mContext.mRenderDev->BindTexture( desc.mPrefilteredEnvMap, LUM_TEX_PREFILTERED );
		gbuffer.BindTextures( );

		mContext.mRenderDev->DrawElements( quad.mVao, 6 );

	}




	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void LightPass::init( ) {

		rhi::BufferCreateInfo desc;
		desc.mBufferUsage = rhi::BufferUsage::Dynamic;
		desc.mMapFlags = rhi::MapFlag::Write;

		// Point Lights SSBO
		if (!mContext.mRenderDev->IsValid( mLightsUBO )) {

			desc.mSize = (sizeof( PointLight ) * LUM_MAX_LIGHTS + sizeof( int32 )) + (sizeof( SpotLight ) * LUM_MAX_LIGHTS + sizeof( int32 ));
			desc.mBufferType = rhi::BufferType::ShaderStorage;
			mLightsUBO = mContext.mRenderDev->CreateBuffer( desc );
			mContext.mRenderDev->SetShaderStorageBinding( mLightsUBO, LUM_SSBO_LIGHTS_BINDING );

		}
		// Directional Light UBO
		if (!mContext.mRenderDev->IsValid( mDirectionalLightUBO )) {

			desc.mSize = sizeof( mDirectionalLightData );
			desc.mBufferType = rhi::BufferType::Uniform;
			mDirectionalLightUBO = mContext.mRenderDev->CreateBuffer( desc );
			mContext.mRenderDev->SetUniformBufferBinding( mDirectionalLightUBO, LUM_UBO_DIRECTIONAL_LIGHT );

		}

		if (!mContext.mRenderDev->IsValid( mPipeline )) {
		
			rhi::PipelineCreateInfo desc;
			desc.mDepthStencil.mDepth.bEnabled = false;
			desc.mDepthStencil.mDepth.bWriteToZBuffer = false;
			mPipeline = mContext.mRenderDev->CreatePipeline( desc );
		
		}

		{ // Shaders

			mShader = mContext.mShaderMgr->LoadShader( "shaders/light_pass.vert", "shaders/light_pass.frag", ResourceRoot::Internal );
		
		}


	}

	void LightPass::upload_point_lights( ) {

		mContext.mRenderDev->UpdateBuffer(
			mLightsUBO, &mActivePointLights,
			skOffsetActivePoint, sizeof( int32 )
		);

		mContext.mRenderDev->UpdateBuffer(
			mLightsUBO, mPointLights.data( ),
			skOffsetPointLights, sizeof( PointLight ) * LUM_MAX_LIGHTS
		);

	}
	void LightPass::upload_spot_lights( ) {

		mContext.mRenderDev->UpdateBuffer(
			mLightsUBO, &mActiveSpotLights,
			skOffsetActiveSpot, sizeof( int32 )
		);

		mContext.mRenderDev->UpdateBuffer(
			mLightsUBO, mSpotLights.data( ),
			skOffsetSpotLights, sizeof( SpotLight ) * LUM_MAX_LIGHTS
		);

	}
	void LightPass::upload_directional_light( ) {

		mContext.mRenderDev->UpdateBuffer(
			mDirectionalLightUBO,
			&mDirectionalLightData
		);

	}

} // namespace lum::render