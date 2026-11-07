/*******************************************************/
/* /                                                 \ */
/*|-------------------|LumEngine|---------------------|*/
/*|            Cross-Platform Game Engine             |*/
/*|===================================================|*/
/*|                    GitHub:                        |*/
/*|       https://github.com/3zymek/LumEngine         |*/
/*|                                                   |*/
/*|        Copyright (c) 2025-present 3zymek          |*/
/*|          Licensed under the MIT License           |*/
/* \                                                 / */
/*******************************************************/
#include "Core/CorePch.hpp"

// Header file impl
#include "Rendering/Renderer.hpp"

#include "Core/CoreTypes.hpp"
#include "Rendering/RenderingTypes.hpp"
#include "Rendering/VertexLayoutConfig.hpp"
#include "Rhi/Interfaces/Core/RhiBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiCommandBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiCommandTypes.hpp"
#include "Rhi/Interfaces/Core/RhiDevice.hpp"
#include "Rhi/Interfaces/Core/RhiImage.hpp"
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"
#include "Rhi/Interfaces/Core/RhiResourceSet.hpp"
#include "Rhi/Interfaces/Core/RhiSampler.hpp"

namespace lum {

	void Renderer::Initialize( const RendererContext& ctx, uint32 initialWidth, uint32 initialHeight ) noexcept {

		ctx.Assert( );

		m_Context = ctx;

		rhi::CommandBuffer initialCmdBuffer{};

		create_screen_quad_image( initialWidth, initialHeight, initialCmdBuffer );
		create_main_pipeline( );
		setup_resources( initialCmdBuffer );

		m_Context.m_RenderDevice( ).SendCommandBuffer( initialCmdBuffer );

	}

	const rhi::ImageHandle Renderer::GetFrameTexture( ) const noexcept {
		return m_ScreenQuad.m_FrameImage;
	}

	void Renderer::SubmitInstance( const RenderInstance& instance ) noexcept {



	}

	void Renderer::BeginFrame( ) {



	}

	void Renderer::EndFrame( ) {

		m_Context.m_RenderDevice( ).SendCommandBuffer( m_CmdBuffer );

	}

	void Renderer::create_screen_quad_image( uint32 width, uint32 height, rhi::CommandBuffer& cmdBuff ) {

		m_Context.m_RenderDevice( ).DestroyImage( m_ScreenQuad.m_FrameImage );

		auto grassPath = ResourceLoader::ResolveResourcePath(
			ResourceRoot::External, "test.png"
		);
		auto grassTexture = ResourceRegistry::GetStatic( ).LoadImageFromFile( grassPath );
		auto grassData = ResourceRegistry::GetStatic( ).GetCachedImage( grassTexture );

		const auto& unpacked = grassData.Value( );

		rhi::ImageCreateInfo info{};
		info.m_ArrayLayers = 1;
		info.m_MipmapLevels = 1;
		info.m_Extent = { unpacked.m_Width, unpacked.m_Height, 1 };
		info.m_Dimension = rhi::ImageDimension::Image2D;
		info.m_Format = rhi::ImageFormat::RGBA8_SRGB;
		info.m_ViewType = rhi::ImageViewType::Default;
		info.m_UsageFlags = rhi::ImageUsageFlag::ShaderReadable | rhi::ImageUsageFlag::TransferDestination;

		m_ScreenQuad.m_FrameImage = m_Context.m_RenderDevice( ).CreateImage( info );

		rhi::cmd::TransferImage transferImg{};
		transferImg.m_Image = m_ScreenQuad.m_FrameImage;
		transferImg.m_DataHandle = grassTexture;

		cmdBuff.Push( transferImg );
	}

	void Renderer::setup_resources( rhi::CommandBuffer& cmdBuffer ) {

		{ // Frame sampler

			rhi::SamplerCreateInfo info{ };
			m_ScreenQuad.m_FrameSampler = m_Context.m_RenderDevice( ).CreateSampler( info );

		}
		{ // VBO
			rhi::BufferCreateInfo info{};
			info.m_Usage = rhi::BufferUsageFlag::Vertex;
			info.m_BufferSize = m_ScreenQuad.s_Vertices.size( ) * sizeof( rhi::Vertex );
			info.m_DataSize = m_ScreenQuad.s_Vertices.size( ) * sizeof( rhi::Vertex );
			info.m_Data = m_ScreenQuad.s_Vertices.data( );
			m_ScreenQuad.m_VertexBuffer = m_Context.m_RenderDevice( ).CreateBuffer( info );
		}
		{ // EBO
			rhi::BufferCreateInfo info{};
			info.m_Usage = rhi::BufferUsageFlag::Element;
			info.m_BufferSize = m_ScreenQuad.s_Indices.size( ) * sizeof( uint32 );
			info.m_DataSize = m_ScreenQuad.s_Indices.size( ) * sizeof( uint32 );
			info.m_Data = m_ScreenQuad.s_Indices.data( );
			m_ScreenQuad.m_ElementBuffer = m_Context.m_RenderDevice( ).CreateBuffer( info );
		}
		{ // Resource set

			rhi::ResourceSetCreateInfo info{};
			info.m_Layout = m_ScreenQuad.m_ResourceLayout;

			m_ScreenQuad.m_ResourceSet = m_Context.m_RenderDevice( ).CreateResourceSet( info );

		}


		{ // Assign frame image and sampler to resource set
			rhi::cmd::UpdateImageInfo imgInfo{};
			imgInfo.m_ArrayIndex = 0;
			imgInfo.m_Binding = 0;
			imgInfo.m_ImageHandle = m_ScreenQuad.m_FrameImage;
			imgInfo.m_SamplerHandle = m_ScreenQuad.m_FrameSampler;

			rhi::cmd::UpdateResourceSet setInfo{};
			setInfo.m_SetHandle = m_ScreenQuad.m_ResourceSet;
			setInfo.m_ImageInfos.PushBack( imgInfo );

			cmdBuffer.Push( setInfo );
		}

		{ // Main renderer loop commands

			rhi::cmd::BindGraphicsPipeline bindPip{
				.m_Handle = m_ScreenQuad.m_Pipeline
			};
			m_CmdBuffer.Push( bindPip );

			rhi::cmd::BindVertexBuffers bindVbo{
				.m_BufferHandles = std::span<rhi::BufferHandle>( &m_ScreenQuad.m_VertexBuffer, 1 )
			};
			m_CmdBuffer.Push( bindVbo );

			rhi::cmd::BindElementBuffer bindEbo{
				.m_BufferHandle = m_ScreenQuad.m_ElementBuffer
			};
			m_CmdBuffer.Push( bindEbo );

			rhi::cmd::SetScissors setScissors{};
			rhi::cmd::SetViewport setViewport{};
			m_CmdBuffer.Push( setScissors );
			m_CmdBuffer.Push( setViewport );

			rhi::cmd::BindResourceSet bindSet{
				.m_SetHandle = m_ScreenQuad.m_ResourceSet
			};
			m_CmdBuffer.Push( bindSet );

			rhi::cmd::DrawIndexed drawIndexed{
				.m_IndexCount = 6,
				.m_InstanceCount = 1,
				.m_FirstIndex = 0,
				.m_VertexOffset = 0
			};
			m_CmdBuffer.Push( drawIndexed );

		}

	}

	void Renderer::create_main_pipeline( ) {


		{ // Resource layout

			// Binding for screen image
			rhi::ResourceSetBinding binding{};
			binding.m_Binding = 0;
			binding.m_ResourceCount = 1;
			binding.m_ShaderStages = rhi::ShaderStageFlag::Fragment;
			binding.m_Type = rhi::ResourceType::CombinedImageSampler;

			rhi::ResourceLayoutCreateInfo info{};
			info.m_Bindings.push_back( binding );

			m_ScreenQuad.m_ResourceLayout = m_Context.m_RenderDevice( ).CreateResourceLayout( info );

		}
		
		{
			rhi::PipelineCreateInfo info{};

			info.m_Layout.m_ResourceLayouts.push_back( m_ScreenQuad.m_ResourceLayout );;

			auto& assembly = info.m_AssemblyPass;
			assembly.m_PrimitiveRestart = false;
			assembly.m_Topology = rhi::PrimitiveTopology::TriangleList;

			auto& rast = info.m_RasterizationPass;
			rast.m_CullMode = rhi::CullMode::Back;
			rast.m_DepthBiasEnabled = false;
			rast.m_PolygonMode = rhi::PolygonMode::Fill;
			rast.m_FrontFace = rhi::FrontFace::Clockwise;
			rast.m_RasterizerEnabled = true;

			auto& blend = info.m_ColorBlendPass;
			rhi::ColorBlendAttachment attachment{};
			attachment.m_BlendEnabled = false;
			attachment.m_ColorMask = rhi::ColorComponentFlag::RGBA;
			blend.m_Attachments.push_back( attachment );

			info.m_VertexInputPass = VertexLayoutConfiguration::GetStatic( ).GetVertexInputPass( );

			rhi::ShaderInfo vertexInfo{};
			vertexInfo.m_EntryPoint = "main";
			vertexInfo.m_Path = ResourceLoader::ResolveResourcePath( ResourceRoot::Internal, "Shaders/screen_quad.vert.spv" );
			vertexInfo.m_Stage = rhi::ShaderStageFlag::Vertex;
			info.m_ShaderInfos.push_back( vertexInfo );

			rhi::ShaderInfo fragmentInfo{};
			fragmentInfo.m_EntryPoint = "main";
			fragmentInfo.m_Path = ResourceLoader::ResolveResourcePath( ResourceRoot::Internal, "Shaders/screen_quad.frag.spv" );
			fragmentInfo.m_Stage = rhi::ShaderStageFlag::Fragment;
			info.m_ShaderInfos.push_back( fragmentInfo );

			m_ScreenQuad.m_Pipeline = m_Context.m_RenderDevice( ).CreatePipeline( info );
		}

	}

} // namespace lum