//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Bridge between engine data and the RHI backend.
//
//=============================================================================//

#include "Core/CoreCommon.hpp"

#include "Render/Renderer.hpp"

#include "Event/EventBus.hpp"
#include "Event/Events/WindowEvents.hpp"

namespace lum::render {

	//=======================================================//
	// Public
	//=======================================================//

	void Renderer::Initialize( const RendererContext& ctx, uint32 w, uint32 h ) {

		ctx.Validate( );
		
		m_Ctx = ctx;

		m_GeometryPass.Initialize( m_Ctx );
		m_LightPass.Initialize( m_Ctx );
		m_EnvironmentPass.Initialize( m_Ctx );
		m_ShadowSys.Initialize( m_Ctx );
		m_PostprocessPass.Initialize( m_Ctx );
		m_DefferedBuffer.Initialize( m_Ctx, 500, 500 );

		init( w, h );

	}

	void Renderer::UpdateCamera( const RenderCamera& camera ) {

		Matrix4 jittered = m_TemporalAa.ApplyJitter( camera.m_Projection );

		m_CameraGpu.m_Position = Vector4( camera.m_Position, 0.0f );
		m_CameraGpu.m_Projection = jittered;
		m_CameraGpu.m_View = camera.m_View;
		m_CameraGpu.m_InvViewProj = Inverse( jittered * camera.m_View );

		upload_camera_uniform( );

	}

	void Renderer::BeginFrame( ) {

		m_LightPass.ClearLights( ); // clear added lights from previous frame
		m_Ctx.m_RenderDev().BindFramebuffer( rhi::k_DefaultFramebuffer ); // bind default framebuffer
		m_Ctx.m_RenderDev().Clear( rhi::BufferBit::Color | rhi::BufferBit::Depth | rhi::BufferBit::Stencil ); // clear default framebuffer

	}

	void Renderer::EndFrame( ) {

		m_Ctx.m_RenderDev().BindFramebuffer( m_ScreenQuad.m_SceneFbo );
		m_Ctx.m_RenderDev().Clear( rhi::BufferBit::Color | rhi::BufferBit::Depth | rhi::BufferBit::Stencil ); // clear scene framebuffer

		m_ShadowSys.Execute( m_GeometryPass, m_LightPass ); // calculate shadows
		m_DefferedBuffer.Clear( );
		m_GeometryPass.Execute( m_DefferedBuffer ); // draw geometry
		
		m_LightPassExecuteCtx.m_IrradianceMap = m_EnvironmentPass.GetTexture( detail::IBLTexture::IrradianceMap );
		m_LightPassExecuteCtx.m_PrefilteredEnvMap = m_EnvironmentPass.GetTexture( detail::IBLTexture::PrefilteredMap );
		m_LightPassExecuteCtx.m_ShadowMap = m_ShadowSys.m_DirectionalLight.GetShadowMap( );

		m_Ctx.m_RenderDev().BindFramebuffer( m_ScreenQuad.m_SceneFbo );
		m_LightPass.Execute( m_DefferedBuffer, m_ScreenQuad, m_LightPassExecuteCtx ); // apply lightning
		
		m_EnvironmentPass.Execute( m_DefferedBuffer, m_ScreenQuad ); // apply skybox

		{
			PostprocessPassExecute desc{};
			m_PostprocessPass.Execute( m_ScreenQuad, desc ); // apply postprocess effects
		}

	}





//=======================================================//
	// Private
//=======================================================//

	void Renderer::init( uint32 w, uint32 h ) {

		m_TemporalAa.Initialize( m_Ctx.m_RenderDev() );

		m_Ctx.m_EventBus( ).SubscribePermanently<EWindowResized>(
			[&]( const EWindowResized& e ) {
				if (e.m_Width <= 0 || e.m_Height <= 0) return;
				m_Ctx.m_RenderDev().SetViewport( 0, 0, e.m_Width, e.m_Height );
				ensure_screenquad_texture( e.m_Width, e.m_Height );
				ensure_screenquad_fbo( );
			}
		);

		// Camera Uniform
		if (!m_Ctx.m_RenderDev().IsValid( m_CameraUbo )) {

			rhi::BufferCreateInfo desc;
			desc.m_BufferUsage = rhi::BufferUsage::Dynamic;
			desc.m_MapFlags = rhi::MapFlag::Write;
			desc.m_Size = sizeof( detail::CameraGPU );
			desc.m_BufferType = rhi::BufferType::Uniform;
			m_CameraUbo = m_Ctx.m_RenderDev().CreateBuffer( desc );
			m_Ctx.m_RenderDev().SetUniformBufferBinding( m_CameraUbo, LUM_UBO_CAMERA_BINDING );

		}

		// Screen quad VBO
		if (!m_Ctx.m_RenderDev().IsValid( m_ScreenQuad.m_Vbo )) {

			std::vector<Vertex> vertices = {
				{ {-1.f, -1.f, 0.f}, {}, {0.f, 0.f}, {}, {} },
				{ { 1.f, -1.f, 0.f}, {}, {1.f, 0.f}, {}, {} },
				{ { 1.f,  1.f, 0.f}, {}, {1.f, 1.f}, {}, {} },
				{ {-1.f,  1.f, 0.f}, {}, {0.f, 1.f}, {}, {} },
			};

			rhi::BufferCreateInfo desc;
			desc.m_BufferUsage = rhi::BufferUsage::Static;
			desc.m_Size = ComputeByteSize( vertices );
			desc.m_BufferType = rhi::BufferType::Vertex;
			desc.m_Data = vertices.data( );
			m_ScreenQuad.m_Vbo = m_Ctx.m_RenderDev( ).CreateBuffer( desc );
		}

		// Screen quad EBO
		if (!m_Ctx.m_RenderDev( ).IsValid( m_ScreenQuad.m_Ebo )) {

			std::vector<uint32> indices = {
				0, 1, 2,
				2, 3, 0
			};

			rhi::BufferCreateInfo desc;
			desc.m_BufferUsage = rhi::BufferUsage::Static;
			desc.m_Size = ComputeByteSize( indices );
			desc.m_BufferType = rhi::BufferType::Element;
			desc.m_Data = indices.data( );
			m_ScreenQuad.m_Ebo = m_Ctx.m_RenderDev( ).CreateBuffer( desc );

		}

		// Screen quad VAO
		if (!m_Ctx.m_RenderDev().IsValid( m_ScreenQuad.m_Vao )) {

			std::vector<rhi::VertexAttribute> attrs( 2 );
			attrs[ 0 ].m_Format = rhi::DataFormat::Vec3;
			attrs[ 0 ].m_RelativeOffset = offsetof( Vertex, m_Position );
			attrs[ 0 ].m_ShaderLocation = LUM_LAYOUT_POSITION;

			attrs[ 1 ].m_Format = rhi::DataFormat::Vec2;
			attrs[ 1 ].m_RelativeOffset = offsetof( Vertex, m_Uv );
			attrs[ 1 ].m_ShaderLocation = LUM_LAYOUT_UV;

			rhi::VertexLayoutCreateInfo desc;
			desc.m_Attributes = attrs;
			desc.m_Stride = sizeof( Vertex );
			m_ScreenQuad.m_Vao = m_Ctx.m_RenderDev().CreateVertexLayout( desc, m_ScreenQuad.m_Vbo );

			m_Ctx.m_RenderDev().AttachElementBufferToLayout( m_ScreenQuad.m_Ebo, m_ScreenQuad.m_Vao );

		}

		ensure_screenquad_texture( 500, 500 );
		ensure_screenquad_fbo( );
	}

	void Renderer::ensure_screenquad_fbo( ) {

		m_Ctx.m_RenderDev().Delete( m_ScreenQuad.m_SceneFbo );
		m_Ctx.m_RenderDev().Delete( m_ScreenQuad.m_PostprocessFbo );

		{
			rhi::FramebufferCreateInfo desc;
			desc.m_ColorTex.push_back( { 0, m_ScreenQuad.m_SceneTex } );
			desc.m_DepthTex = m_DefferedBuffer.GetAttachment( detail::DeferredBufferAttachment::Depth );
			m_ScreenQuad.m_SceneFbo = m_Ctx.m_RenderDev().CreateFramebuffer( desc );
		}
		{
			rhi::FramebufferCreateInfo desc;
			desc.m_ColorTex.push_back( { 0, m_ScreenQuad.m_PostprocessTex } );
			desc.m_DepthTex = m_DefferedBuffer.GetAttachment( detail::DeferredBufferAttachment::Depth );
			m_ScreenQuad.m_PostprocessFbo = m_Ctx.m_RenderDev().CreateFramebuffer( desc );
		}

	}
	void Renderer::ensure_screenquad_texture( uint32 w, uint32 h ) {

		m_Ctx.m_RenderDev().Delete( m_ScreenQuad.m_SceneTex );
		m_Ctx.m_RenderDev().Delete( m_ScreenQuad.m_PostprocessTex );

		{
			rhi::TextureCreateInfo desc;
			desc.m_PixelFormat = rhi::PixelLayout::RGBA;
			desc.m_InternalFormat = rhi::TextureFormat::RGBA16F;
			desc.m_TextureType = rhi::TextureKind::Texture2D;
			desc.m_Width = w;
			desc.m_Height = h;
			m_ScreenQuad.m_SceneTex = m_Ctx.m_RenderDev().CreateTexture( desc );
			m_TemporalAa.EnsureFrameTex( desc );
		}
		{
			rhi::TextureCreateInfo desc;
			desc.m_PixelFormat = rhi::PixelLayout::RGBA;
			desc.m_InternalFormat = rhi::TextureFormat::RGBA16F;
			desc.m_TextureType = rhi::TextureKind::Texture2D;
			desc.m_Width = w;
			desc.m_Height = h;
			m_ScreenQuad.m_PostprocessTex = m_Ctx.m_RenderDev().CreateTexture( desc );
		}

	}

	void Renderer::upload_camera_uniform( ) {

		m_Ctx.m_RenderDev().UpdateBuffer( m_CameraUbo, &m_CameraGpu );

	}

}