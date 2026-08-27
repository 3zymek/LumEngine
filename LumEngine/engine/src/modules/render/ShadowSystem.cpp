
#include "Render/ShadowSystem.hpp"
#include "Render/Passes/GeometryPass.hpp"
#include "Render/Passes/LightPass.hpp"
#include "Render/ShaderManager.hpp"

namespace lum::render {

//=======================================================//
	// Public
//=======================================================//

	void ShadowSystem::Initialize( RendererContext& ctx ) {

		m_Ctx = ctx;
		m_DirectionalLight.Initialize( m_Ctx() );

		{ // Shadow mapping pipeline

			rhi::PipelineCreateInfo desc;
			desc.m_DepthStencil.m_Depth.m_Enabled = true;
			desc.m_Cull.m_Enabled = true;
			desc.m_Cull.m_Face = rhi::Face::Back;
			m_ShadowMappingPipeline = m_Ctx().m_RenderDev( ).CreatePipeline( desc );

		}

	}

	void ShadowSystem::Execute( GeometryPass& geoPass, const LightPass& lightPass ) {

		m_Ctx().m_RenderDev( ).BindPipeline( m_ShadowMappingPipeline );
		m_DirectionalLight.Execute( geoPass, lightPass, m_Ctx() );
		//m_SpotLight.Execute( geoPass, lightPass, m_Ctx );

	}

	void ShadowSystem::DirectionalLight::Initialize( RendererContext& ctx ) {

		{ // Shadow map texture
			rhi::TextureCreateInfo desc;
			desc.m_Width = m_ShadowMapTexSize.m_X;
			desc.m_Height = m_ShadowMapTexSize.m_Y;
			desc.m_PixelFormat = rhi::PixelLayout::DepthComponent;
			desc.m_InternalFormat = rhi::TextureFormat::Depth32F;
			desc.m_TextureType = rhi::TextureKind::Texture2D;
			m_ShadowMap = ctx.m_RenderDev( ).CreateTexture( desc );
		}
		{ // Shadow FBO
			rhi::FramebufferCreateInfo desc;
			desc.m_DepthTex = m_ShadowMap;
			m_Framebuffer = ctx.m_RenderDev( ).CreateFramebuffer( desc );
		}
		{ // Light space matrices UBO
			rhi::BufferCreateInfo desc;
			desc.m_BufferType = rhi::BufferType::Uniform;
			desc.m_BufferUsage = rhi::BufferUsage::Dynamic;
			desc.m_MapFlags = rhi::MapFlag::Write;
			desc.m_Size = sizeof( Matrix4 );
			m_LightSpaceUbo = ctx.m_RenderDev( ).CreateBuffer( desc );
			ctx.m_RenderDev( ).SetUniformBufferBinding( m_LightSpaceUbo, LUM_UBO_LIGHTSPACE_MATRIX );
		}
		{ // Shaders
			m_Shader = ctx.m_ShaderMgr( ).LoadShader( "shaders/shadow_mapping/directional.vert", "shaders/shadow_mapping/directional.frag", ResourceRoot::Internal );
		}

	}

	void ShadowSystem::SpotLight::Initialize( RendererContext& ctx ) {

		{ // Shadow maps texture
			rhi::TextureCreateInfo desc;
			desc.m_TextureType = rhi::TextureKind::Texture2DArray;
			desc.m_Depth = limits::k_MaxShadowCastingSpotLights;
			desc.m_Width = m_ShadowMapTexSize.m_X;
			desc.m_Height = m_ShadowMapTexSize.m_Y;
			m_ShadowMaps = ctx.m_RenderDev().CreateTexture( desc );
		}
		{ // Shadow FBO
			rhi::FramebufferCreateInfo desc;
			desc.m_DepthTex = m_ShadowMaps;
			m_Framebuffer = ctx.m_RenderDev( ).CreateFramebuffer( desc );
		}
		{ // Light space matrices UBO
			rhi::BufferCreateInfo desc;
			desc.m_BufferType = rhi::BufferType::Uniform;
			desc.m_BufferUsage = rhi::BufferUsage::Dynamic;
			desc.m_MapFlags = rhi::MapFlag::Write;
			desc.m_Size = sizeof( Matrix4 ) * limits::k_MaxShadowCastingSpotLights;
			m_LightSpaceUbo = ctx.m_RenderDev( ).CreateBuffer( desc );
			ctx.m_RenderDev( ).SetUniformBufferBinding( m_LightSpaceUbo, LUM_UBO_LIGHTSPACE_MATRIX );
		}
		{ // Shader
			m_Shader = ctx.m_ShaderMgr( ).LoadShader( "shaders/shadow_mapping/spot.vert", "shaders/shadow_mapping/spot.frag", ResourceRoot::Internal );
		}

	}


	void ShadowSystem::DirectionalLight::Execute( GeometryPass& geoPass, const LightPass& lightPass, RendererContext& ctx ) {

		auto& device = ctx.m_RenderDev( );

		rhi::ViewportState viewport = device.GetViewport( );

		calculate_lightspace_matrix( lightPass.GetDirectionalLight( ).m_Direction, ctx );
		device.BindFramebuffer( m_Framebuffer );
		device.SetViewport( 0, 0, m_ShadowMapTexSize.m_X, m_ShadowMapTexSize.m_Y );
		device.ClearDepth( );
		
		device.BindShader( m_Shader );

		geoPass.DrawScene( );

		device.BindFramebuffer( rhi::k_DefaultFramebuffer );
		device.SetViewport( 0, 0, viewport.m_Width, viewport.m_Height );

	}

	void ShadowSystem::SpotLight::Execute( GeometryPass& geoPass, const LightPass& lightPass, RendererContext& ctx ) {

		/*
		const auto& spotLights = lightPass.GetSpotLights( );
		for (uint32 i = 0; i < spotLights.second; i++) {

			const auto& light = spotLights.first[ i ];
			

		}*/


	}



//=======================================================//
	// Private
//=======================================================//

	void ShadowSystem::DirectionalLight::calculate_lightspace_matrix( const Vector3& dir, RendererContext& ctx ) {

		Vector3 lightDir = Normalize( dir );
		Vector3 up = Vector3( 0.0f, 1.0f, 0.0f );

		if (Abs( Dot( lightDir, up ) ) > 0.99f)
			up = Vector3( 0.0f, 0.0f, 1.0f );

		Vector3 lightPos = -lightDir * m_ShadowMapDistance;

		Matrix4 lightViewMatrix = LookAt(
			lightPos,
			Vector3( 0.0f ),
			up
		);

		Matrix4 lightProjectionMatrix = Orthographic(
			-m_ShadowMapSize, m_ShadowMapSize,
			-m_ShadowMapSize, m_ShadowMapSize,
			m_ShadowMapNear, m_ShadowMapFar
		);

		upload_lightspace_matrix( lightProjectionMatrix * lightViewMatrix, ctx );

	}

	void ShadowSystem::SpotLight::calculate_lightspace_matrix( const Vector3& dir, const Vector3& pos, float32 fov, RendererContext& ctx ) {

		Vector3 lightDir = Normalize( dir );
		Vector3 up = Vector3( 0.0f, 1.0f, 0.0f );

		if (Abs( Dot( lightDir, up ) ) > 0.99f)
			up = Vector3( 0.0f, 0.0f, 1.0f );

		Matrix4 lightViewMatrix = LookAt(
			pos,
			pos + lightDir,
			up
		);

		float32 aspectRatio = SafeCast<float32>( m_ShadowMapTexSize.m_X ) / SafeCast<float32>( m_ShadowMapTexSize.m_Y );

		Matrix4 lightProjectionMatrix = Perspective(
			fov,
			aspectRatio,
			m_ShadowMapNear,
			m_ShadowMapFar
		);

		upload_lightspace_matrix( lightProjectionMatrix * lightViewMatrix, ctx );

	}

	void ShadowSystem::DirectionalLight::upload_lightspace_matrix( const Matrix4& mat, RendererContext& ctx ) {

		ctx.m_RenderDev().UpdateBuffer(
			m_LightSpaceUbo,
			mat.Data()
		);

	}

	void ShadowSystem::SpotLight::upload_lightspace_matrix( const Matrix4& mat, RendererContext& ctx ) {

		

	}

}