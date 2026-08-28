//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Light render pass — handles point and directional light
//          submission, uniform uploads and lighting pipeline management.
//
//=============================================================================//

#include "Rhi/Core/RhiDevice.hpp"
#include "Render/Renderer.hpp"
#include "Render/ShaderManager.hpp"

namespace lum::render {

//=======================================================//
	// Public
//=======================================================//

	void LightPass::Initialize( RendererContext& ctx ) {

		ctx.Validate( );

		m_Ctx = ctx;

		init( );

	}

	void LightPass::AddPointLight( const PointLight& light ) {

		LUM_ASSERT( m_ActivePointLights + 1 <= m_PointLights.size( ), "Max point lights reached" );

		m_PointLights[ m_ActivePointLights++ ] = light;

	}
	void LightPass::AddSpotLight( const SpotLight& light ) {

		LUM_ASSERT( m_ActiveSpotLights + 1 <= m_SpotLights.size( ), "Max spot lights reached" );

		m_SpotLights[ m_ActiveSpotLights++ ] = light;

	}


	void LightPass::SetDirectionalLight( const DirectionalLight& light ) {

		m_DirectionalLightData.m_Color = Vector4( light.m_Color, 0.0f );
		m_DirectionalLightData.m_Direction = Vector4( light.m_Direction, 0.0f );
		m_DirectionalLightData.m_Intensity = light.m_Intensity;

	}

	DirectionalLight LightPass::GetDirectionalLight( ) {
		return {
			Vector3( m_DirectionalLightData.m_Direction ),
			m_DirectionalLightData.m_Intensity,
			Vector3( m_DirectionalLightData.m_Color )
		};
	}
	DirectionalLight LightPass::GetDirectionalLight( ) const {
		return {
			Vector3( m_DirectionalLightData.m_Direction ),
			m_DirectionalLightData.m_Intensity,
			Vector3( m_DirectionalLightData.m_Color )
		};
	}

	void LightPass::Execute( const detail::DeferredBuffer& gbuffer, const detail::ScreenQuad& quad, const LightPassExectueContext& desc ) {

		auto& device = m_Ctx( ).m_RenderDev( );

		device.BindPipeline( m_Pipeline );

		upload_directional_light( );
		upload_point_lights( );
		upload_spot_lights( );

		device.BindShader( m_Shader );
		device.BindTexture( desc.m_ShadowMap, LUM_SHADOW_MAP );
		device.BindTexture( desc.m_IrradianceMap, LUM_TEX_IRRADIANCE );
		device.BindTexture( desc.m_PrefilteredEnvMap, LUM_TEX_PREFILTERED );
		gbuffer.BindTextures( );

		device.DrawElements( quad.m_Vao, 6 );

	}




//=======================================================//
	// Private
//=======================================================//

	void LightPass::init( ) {

		rhi::BufferCreateInfo desc;
		desc.m_BufferUsage = rhi::BufferUsage::Dynamic;
		desc.m_MapFlags = rhi::MapFlag::Write;

		auto& device = m_Ctx( ).m_RenderDev( );

		// Point Lights SSBO
		if (!device.IsValid( m_LightsUbo )) {

			desc.m_Size = (sizeof( PointLight ) * LUM_MAX_LIGHTS + sizeof( int32 )) + (sizeof( SpotLight ) * LUM_MAX_LIGHTS + sizeof( int32 ));
			desc.m_BufferType = rhi::BufferType::ShaderStorage;
			m_LightsUbo = device.CreateBuffer( desc );
			device.SetShaderStorageBinding( m_LightsUbo, LUM_SSBO_LIGHTS_BINDING );

		}
		// Directional Light UBO
		if (!device.IsValid( m_DirectionalLightUbo )) {

			desc.m_Size = sizeof( m_DirectionalLightData );
			desc.m_BufferType = rhi::BufferType::Uniform;
			m_DirectionalLightUbo = device.CreateBuffer( desc );
			device.SetUniformBufferBinding( m_DirectionalLightUbo, LUM_UBO_DIRECTIONAL_LIGHT );

		}

		if (!device.IsValid( m_Pipeline )) {
		
			rhi::PipelineCreateInfo desc;
			desc.m_DepthStencil.m_Depth.m_Enabled = false;
			desc.m_DepthStencil.m_Depth.m_WriteToZBuffer = false;
			m_Pipeline = m_Ctx( ).m_RenderDev( ).CreatePipeline( desc );
		
		}

		{ // Shaders

			m_Shader = m_Ctx( ).m_Shaderm_Gr( ).LoadShader( "shaders/light_pass.vert", "shaders/light_pass.frag", ResourceRoot::Internal );
		
		}


	}

	void LightPass::upload_point_lights( ) {

		m_Ctx( ).m_RenderDev().UpdateBuffer(
			m_LightsUbo, &m_ActivePointLights,
			sk_OffsetActivePoint, sizeof( int32 )
		);

		m_Ctx( ).m_RenderDev( ).UpdateBuffer(
			m_LightsUbo, m_PointLights.data( ),
			sk_OffsetPointLights, sizeof( PointLight ) * LUM_MAX_LIGHTS
		);

	}
	void LightPass::upload_spot_lights( ) {

		m_Ctx( ).m_RenderDev( ).UpdateBuffer(
			m_LightsUbo, &m_ActiveSpotLights,
			sk_OffsetActiveSpot, sizeof( int32 )
		);

		m_Ctx( ).m_RenderDev( ).UpdateBuffer(
			m_LightsUbo, m_SpotLights.data( ),
			sk_OffsetSpotLights, sizeof( SpotLight ) * LUM_MAX_LIGHTS
		);

	}
	void LightPass::upload_directional_light( ) {

		m_Ctx( ).m_RenderDev( ).UpdateBuffer(
			m_DirectionalLightUbo,
			&m_DirectionalLightData
		);

	}

} // namespace lum::render