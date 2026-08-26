//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Environment render pass — renders the HDR skybox cubemap
//          and precomputes IBL (Image-Based Lighting) maps:
//          irradiance map and prefiltered environment map.
//
//=============================================================================//
#pragma once

#include "Render/Passes/EnvironmentPass.hpp"
#include "Render/RenderCommon.hpp"
#include "Rhi/Core/RhiDevice.hpp"
#include "Render/Renderer.hpp"
#include "Render/ShaderManager.hpp"

namespace lum::render {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void EnvironmentPass::Initialize( RendererContext& ctx ) {

		ctx.Validate( );

		m_Ctx = ctx;

		init( );

	}

	void EnvironmentPass::SetCubemapTexture( rhi::TextureHandle tex ) {
		m_Cubemap.m_Texture = tex;
		generate_irradiance_map( );
		generate_prefiltered_map( );
	}

	void EnvironmentPass::Execute( detail::DeferredBuffer& gbuffer, const detail::ScreenQuad& quad ) {

		auto& device = m_Ctx().m_RenderDev( );

		device.BindFramebuffer( quad.m_SceneFbo ); // Render skybox to screenquad
		device.BindPipeline( m_Cubemap.m_Pipeline );
		device.BindShader( m_Cubemap.m_Shader );
		device.BindTexture( m_Cubemap.m_Texture, LUM_TEX_CUBEMAP );
		device.BindSampler( m_Sampler, LUM_TEX_CUBEMAP );

		device.DrawElements( m_Cubemap.m_Vao, m_Cubemap.m_NumIndices );

	}

	rhi::TextureHandle EnvironmentPass::GetTexture( detail::IBLTexture tex ) const noexcept {
		switch (tex) {
		case detail::IBLTexture::IrradianceMap: return mIBL.m_Irradiance.m_Texture;
		case detail::IBLTexture::PrefilteredMap: return mIBL.m_Prefiltered.m_Texture;
		default: return { };
		}
		return {};
	}



	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void EnvironmentPass::generate_irradiance_map( ) {

		rhi::BufferHandle captureUBO;
		auto& device = m_Ctx().m_RenderDev( );

		// Capture UBO (IBL)s
		{

			rhi::BufferCreateInfo desc;
			desc.m_BufferType = rhi::BufferType::Uniform;
			desc.m_BufferUsage = rhi::BufferUsage::Dynamic;
			desc.m_MapFlags = rhi::MapFlag::Write;
			desc.m_Size = sizeof( Matrix4 ) * 2;
			captureUBO = device.CreateBuffer( desc );
			device.SetUniformBufferBinding( captureUBO, LUM_UBO_IRRADIANCE );

		}

		rhi::FramebufferHandle		captureFBO = device.CreateFramebuffer( { } );
		Matrix4						captureProjection = IBL::GetCaptureProjection( );
		std::array<Matrix4, 6>		captureViews = IBL::GetCaptureViews( );
		rhi::ViewportState			viewport = device.GetViewport( );

		device.SetViewport( 0, 0, 32, 32 );
		device.BindFramebuffer( captureFBO );
		device.BindTexture( m_Cubemap.m_Texture, LUM_TEX_CUBEMAP );
		device.BindSampler( m_Sampler, LUM_TEX_CUBEMAP );
		device.BindPipeline( m_Cubemap.m_Pipeline );
		device.BindShader( mIBL.m_Irradiance.m_Shader );

		for (int32 i = 0; i < 6; i++) {

			Matrix4 matrices[ ] = { captureProjection, captureViews[ i ] };
			device.UpdateBuffer( captureUBO, matrices );

			rhi::TextureLayerAttachment attach;
			attach.m_Attachment = rhi::BufferBit::Color;
			attach.m_Mip = 1;
			attach.m_Layer = i;
			attach.m_Slot = 0;
			device.AttachTextureLayer( captureFBO, mIBL.m_Irradiance.m_Texture, attach );
			device.DrawElements( m_Cubemap.m_Vao, m_Cubemap.m_NumIndices );

		}

		device.Delete( captureFBO );
		device.Delete( captureUBO );

		device.SetViewport( 0, 0, viewport.m_Width, viewport.m_Height );

	}

	void EnvironmentPass::generate_prefiltered_map( ) {

		rhi::BufferHandle captureUBO;

		auto& device = m_Ctx().m_RenderDev( );

		// Capture UBO (IBL)
		{

			rhi::BufferCreateInfo desc;
			desc.m_BufferType = rhi::BufferType::Uniform;
			desc.m_BufferUsage = rhi::BufferUsage::Dynamic;
			desc.m_MapFlags = rhi::MapFlag::Write;
			desc.m_Size = sizeof( Matrix4 ) * 2 + sizeof( float32 );
			captureUBO = device.CreateBuffer( desc );
			device.SetUniformBufferBinding( captureUBO, LUM_UBO_PREFILTERED_MAP );

		}

		rhi::FramebufferHandle		captureFBO = device.CreateFramebuffer( { } );
		Matrix4						captureProjection = IBL::GetCaptureProjection( );
		std::array<Matrix4, 6>		captureViews = IBL::GetCaptureViews( );
		rhi::ViewportState			viewport = device.GetViewport( );

		device.BindFramebuffer( captureFBO );
		device.BindTexture( m_Cubemap.m_Texture, LUM_TEX_CUBEMAP );
		device.BindSampler( m_Sampler, LUM_TEX_CUBEMAP );
		device.BindPipeline( m_Cubemap.m_Pipeline );
		device.BindShader( mIBL.m_Prefiltered.m_Shader );

		struct LUM_UBO_ALIGNMENT UniformData {
			Matrix4 m_Projection = Matrix4( 1.0f );
			Matrix4 m_View = Matrix4( 1.0f );
			float32 m_Roughness{};
			float32 _pad[ 3 ]{};
		};

		for (uint32 mip = 0; mip < mIBL.m_Prefiltered.skMipmapLevels; mip++) {

			float32 roughness = SafeCast<float32>( mip ) / SafeCast<float32>( (mIBL.m_Prefiltered.skMipmapLevels - 1) );

			uint32 mipSize = 128 >> mip;
			device.SetViewport( 0, 0, mipSize, mipSize );

			for (int32 i = 0; i < 6; i++) {

				UniformData data{};
				data.m_Projection = captureProjection;
				data.m_View = captureViews[ i ];
				data.m_Roughness = roughness;

				device.UpdateBuffer( captureUBO, &data );

				rhi::TextureLayerAttachment attach;
				attach.m_Attachment = rhi::BufferBit::Color;
				attach.m_Mip = mip;
				attach.m_Layer = i;
				attach.m_Slot = 0;
				device.AttachTextureLayer( captureFBO, mIBL.m_Prefiltered.m_Texture, attach );
				device.DrawElements( m_Cubemap.m_Vao, m_Cubemap.m_NumIndices );

			}

		}

		device.Delete( captureFBO );
		device.Delete( captureUBO );

		device.SetViewport( 0, 0, viewport.m_Width, viewport.m_Height );

	}

	void EnvironmentPass::init( ) {

		float32 cubemapVertices[ ] = {

			-1, -1, -1,  1, -1, -1,  1,  1, -1, -1,  1, -1,
			-1, -1,  1,  1, -1,  1,  1,  1,  1, -1,  1,  1,

		};

		uint32 cubemapIndices[ ] = {

			0,1,2, 2,3,0, // front
			5,4,7, 7,6,5, // back
			4,0,3, 3,7,4, // left
			1,5,6, 6,2,1, // right
			3,2,6, 6,7,3, // top
			4,5,1, 1,0,4  // bottom

		};

		m_Cubemap.m_NumIndices = ArraySize( cubemapIndices );

		auto& device = m_Ctx().m_RenderDev( );

		// Cubemap VBO
		if (!device.IsValid( m_Cubemap.m_Vbo )) {

			rhi::BufferCreateInfo desc;
			desc.m_BufferUsage = rhi::BufferUsage::Static;
			desc.m_MapFlags = rhi::MapFlag::None;
			desc.m_Size = ComputeByteSize( cubemapVertices );
			desc.m_Data = cubemapVertices;
			desc.m_BufferType = rhi::BufferType::Vertex;
			m_Cubemap.m_Vbo = device.CreateBuffer( desc );

		}

		// Cubemap EBO
		if (!device.IsValid( m_Cubemap.m_Ebo )) {

			rhi::BufferCreateInfo desc;
			desc.m_BufferUsage = rhi::BufferUsage::Static;
			desc.m_MapFlags = rhi::MapFlag::None;
			desc.m_Size = ComputeByteSize( cubemapIndices );
			desc.m_Data = cubemapIndices;
			desc.m_BufferType = rhi::BufferType::Element;
			m_Cubemap.m_Ebo = device.CreateBuffer( desc );

		}

		// Cubemap VAO
		if (!device.IsValid( m_Cubemap.m_Vao )) {

			rhi::VertexAttribute attrs[ ]{
				{
					.m_Format = rhi::DataFormat::Vec3,
					.m_RelativeOffset = 0,
					.m_ShaderLocation = LUM_LAYOUT_POSITION
				}
			};
			rhi::VertexLayoutCreateInfo desc;
			desc.m_Stride = 3 * sizeof( float32 );
			desc.m_Attributes = attrs;
			m_Cubemap.m_Vao = device.CreateVertexLayout( desc, m_Cubemap.m_Vbo );
			device.AttachElementBufferToLayout( m_Cubemap.m_Ebo, m_Cubemap.m_Vao );

		}

		// Cubemap sampler
		if (!device.IsValid( m_Sampler )) {

			rhi::SamplerCreateInfo desc;
			desc.m_MinFilter = rhi::SamplerMinFilter::LinearMipmapLinear;
			desc.m_MagFilter = rhi::SamplerMagFilter::Linear;
			desc.m_WrapR = rhi::SamplerWrap::ClampEdge;
			desc.m_WrapS = rhi::SamplerWrap::ClampEdge;
			desc.m_WrapT = rhi::SamplerWrap::ClampEdge;
			m_Sampler = device.CreateSampler( desc );

		}

		// Cubemap Pipeline
		if (!device.IsValid( m_Cubemap.m_Pipeline )) {

			rhi::PipelineCreateInfo desc;
			desc.m_DepthStencil.m_Depth.bEnabled = true;
			desc.m_DepthStencil.m_Depth.bWriteToZBuffer = false;
			desc.m_DepthStencil.m_Depth.m_Compare = rhi::CompareFlag::LessEqual;
			desc.m_Cull.bEnabled = false;
			desc.m_Cull.m_Face = rhi::Face::Back;
			m_Cubemap.m_Pipeline = device.CreatePipeline( desc );

		}

		// Irradiance map (IBL)
		if (!device.IsValid( mIBL.m_Irradiance.m_Texture )) {

			rhi::TextureCreateInfo desc;
			desc.m_TextureType = rhi::TextureKind::Cubemap;
			desc.m_InternalFormat = rhi::TextureFormat::RGB16F;
			desc.m_PixelFormat = rhi::PixelLayout::RGB;
			desc.m_DataType = rhi::PixelDataType::Float;
			desc.m_Width = 32;
			desc.m_Height = 32;
			mIBL.m_Irradiance.m_Texture = device.CreateTexture( desc );

		}

		// Prefiltered environment map (IBL)
		if (!device.IsValid( mIBL.m_Prefiltered.m_Texture )) {

			rhi::TextureCreateInfo desc;
			desc.m_TextureType = rhi::TextureKind::Cubemap;
			desc.m_PixelFormat = rhi::PixelLayout::RGB;
			desc.m_InternalFormat = rhi::TextureFormat::RGB16F;
			desc.m_Width = 128;
			desc.m_Height = 128;
			desc.m_MipmapLevels = 5;
			mIBL.m_Prefiltered.m_Texture = device.CreateTexture( desc );

		}
		{ // Shaders

			m_Cubemap.m_Shader = m_Ctx().m_ShaderMgr().LoadShader( "shaders/skybox_pass.vert", "shaders/skybox_pass.frag", ResourceRoot::Internal );

			mIBL.m_Irradiance.m_Shader = m_Ctx().m_ShaderMgr( ).LoadShader( "shaders/irradiance.vert", "shaders/irradiance.frag", ResourceRoot::Internal );
			mIBL.m_Prefiltered.m_Shader = m_Ctx().m_ShaderMgr( ).LoadShader( "shaders/prefiltered_env.vert", "shaders/prefiltered_env.frag", ResourceRoot::Internal );

		}

	}


} // namespace lum::render