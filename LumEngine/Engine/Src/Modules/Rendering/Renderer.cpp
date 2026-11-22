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
#include "Core/Math/MatrixOperations.hpp"
#include "Entity/Components/Material.hpp"
#include "Entity/Components/Mesh.hpp"
#include "Entity/Components/Transform.hpp"
#include "Event/EventBus.hpp"
#include "Event/Events/WindowEvents.hpp"
#include "Rendering/DeferredBuffer.hpp"
#include "Rendering/Passes/GeometryPass.hpp"
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

	void Renderer::Initialize( const RendererContext& ctx, ev::EventBus& bus, uint32 initialWidth, uint32 initialHeight ) noexcept {

		ctx.Assert( );
		m_Ctx = ctx;

		rhi::Extent2D swapchainExt = m_Ctx.m_RenderDevice( ).GetSwapchainExtent( );
		m_DeferredBuffer.RecreatePrimitives( m_Ctx.m_RenderDevice, swapchainExt );

		m_GeometryPass.Initialize( m_Ctx, m_DeferredBuffer );
		m_LightPass.Initialize( m_Ctx, m_DeferredBuffer );

		rhi::CommandBuffer initialCmdBuffer{};

		bus.SubscribePermanently<EWindowResized>(
			[ & ]( const EWindowResized& ev ) {
				rhi::CommandBuffer buffer{};
				m_DeferredBuffer.RecreatePrimitives( m_Ctx.m_RenderDevice, { ev.m_Width, ev.m_Height } );
				m_LightPass.RefreshResources( );
				this->create_screen_quad_primitives( ev.m_Width, ev.m_Height, buffer );
				m_Ctx.m_RenderDevice( ).SendCommandBuffer( buffer );
			}
		);

		create_screen_quad_primitives( initialWidth, initialHeight, initialCmdBuffer );
		create_main_pipeline( );
		setup_resources( initialCmdBuffer );

		m_Ctx.m_RenderDevice( ).SendCommandBuffer( initialCmdBuffer );

	}

	const rhi::ImageHandle Renderer::GetFrameTexture( ) const noexcept {
		return m_DeferredBuffer.GetAttachment( DeferredAttachment::Lit );
	}

	void Renderer::SubmitInstance( const RenderInstance& instance ) noexcept {

		m_GeometryPass.SubmitInstance( instance );

	}

	void Renderer::SubmitCameraData( const CameraData& data ) noexcept {
		m_GeometryPass.SubmitCameraData( data );
		m_LightPass.SubmitCameraData( data );
	}

	void Renderer::BeginFrame( ) {



	}

	void Renderer::EndFrame( ) {

		if (!m_Ctx.m_RenderDevice( ).BeginFrame( ))
			return;


		// DEBUG

		static OwningPtr<CStaticMesh> mesh = OwningPtr<CStaticMesh>::Create( );

		static bool triangleLoaded = false;
		if (!triangleLoaded) {
			auto handle = ResourceRegistry::GetStatic( ).LoadResourceFromFile<ResourceType::Mesh>(
				ResourceLoader::ResolveResourcePath( ResourceRoot::External, "TestTriangle.obj" )
			);
			mesh( ).m_ResourceHandle = handle;
			triangleLoaded = true;
		}

		static OwningPtr<CStaticMesh> rectangleMesh = OwningPtr<CStaticMesh>::Create( );
		static bool rectangleLoaded = false;
		if (!rectangleLoaded) {
			auto handle = ResourceRegistry::GetStatic( ).LoadResourceFromFile<ResourceType::Mesh>(
				ResourceLoader::ResolveResourcePath( ResourceRoot::External, "TestRectangle.obj" )
			);
			rectangleMesh( ).m_ResourceHandle = handle;
			rectangleLoaded = true;
		}

		static OwningPtr<CTransform> transform = OwningPtr<CTransform>::Create( );

		static OwningPtr<CMaterialInstance> material = OwningPtr<CMaterialInstance>::Create( );
		material( ).m_BaseColor = Vector3( 1.0f, 0.0f, 0.0f );

		RenderInstance instance{};
		instance.m_Material = material;
		instance.m_StaticMesh = mesh;
		instance.m_Transform = transform;

		m_GeometryPass.SubmitInstance( instance );

		static OwningPtr<CTransform> rectangleTransform = OwningPtr<CTransform>::Create( );
		static OwningPtr<CMaterialInstance> rectangleMaterial = OwningPtr<CMaterialInstance>::Create( );
		static bool rectangleTransformInitialized = false;
		if (!rectangleTransformInitialized) {
			rectangleTransform( ).m_Position = Vector3( 1.5f, 0.0f, -1.4f );
			rectangleTransform( ).m_WorldMatrix = Translate( Matrix4( 1.0f ), rectangleTransform( ).m_Position );
			rectangleTransformInitialized = true;
		}

		rectangleMaterial( ).m_BaseColor = Vector3( 0.0f, 1.0f, 1.0f );
		RenderInstance rectangleInstance{};
		rectangleInstance.m_Material = rectangleMaterial;
		rectangleInstance.m_StaticMesh = rectangleMesh;
		rectangleInstance.m_Transform = rectangleTransform;
		m_GeometryPass.SubmitInstance( rectangleInstance );

		// DEBUG

		m_GeometryPass.DrawScene( );
		m_LightPass.DrawScene( );

		rhi::CommandBuffer cmdBuffer{};

		rhi::RenderingAttachment swapchainAttachment{};
		swapchainAttachment.m_Image = m_Ctx.m_RenderDevice( ).GetCurrentSwapchainImage( );
		swapchainAttachment.m_StoreOp = rhi::StoreOp::Store;
		swapchainAttachment.m_LoadOp = rhi::LoadOp::Clear;
		swapchainAttachment.m_ClearValue = rhi::RenderingAttachment::ClearValue::Color( { 0.0f, 0.0f, 0.0f, 0.0f } );

		rhi::cmd::BeginRendering beginRendering{};
		beginRendering.m_RenderArea = m_Ctx.m_RenderDevice( ).GetSwapchainExtent( );
		beginRendering.m_ColorAttachments.PushBack( swapchainAttachment );
		beginRendering.m_LayerCount = 1;

		rhi::cmd::TransitionImage swapchainAsAttachment{};
		swapchainAsAttachment.m_Image = swapchainAttachment.m_Image;
		swapchainAsAttachment.m_NewUse = rhi::ImageUse::ColorAttachment;
		cmdBuffer.Push( swapchainAsAttachment );

		rhi::cmd::TransitionImage sampledAlbedo{};
		sampledAlbedo.m_Image = m_DeferredBuffer.GetAttachment( DeferredAttachment::Lit);
		sampledAlbedo.m_NewUse = rhi::ImageUse::Sampled;
		sampledAlbedo.m_ShaderStages = rhi::ShaderStageFlag::Fragment;
		cmdBuffer.Push( sampledAlbedo );

		cmdBuffer.Push( beginRendering );

		rhi::cmd::BindGraphicsPipeline bindPip{
			.m_Handle = m_ScreenQuad.m_Pipeline
		};
		cmdBuffer.Push( bindPip );

		rhi::cmd::BindVertexBuffers bindVbo{};
		bindVbo.m_BufferHandles.PushBack( m_ScreenQuad.m_VertexBuffer );
		cmdBuffer.Push( bindVbo );

		rhi::cmd::BindElementBuffer bindEbo{
			.m_BufferHandle = m_ScreenQuad.m_ElementBuffer
		};
		cmdBuffer.Push( bindEbo );

		rhi::cmd::SetScissors setScissors{};
		rhi::cmd::SetViewport setViewport{};
		cmdBuffer.Push( setScissors );
		cmdBuffer.Push( setViewport );

		rhi::cmd::BindResourceSet bindSet{
			.m_SetHandle = m_ScreenQuad.m_ResourceSet
		};
		cmdBuffer.Push( bindSet );

		rhi::cmd::DrawIndexed drawIndexed{
			.m_IndexCount = 6,
			.m_InstanceCount = 1,
			.m_FirstIndex = 0,
			.m_VertexOffset = 0
		};
		cmdBuffer.Push( drawIndexed );

		cmdBuffer.Push( rhi::cmd::EndRendering{} );

		m_Ctx.m_RenderDevice( ).SendCommandBuffer( cmdBuffer );

		m_Ctx.m_RenderDevice( ).EndFrame( );

	}

	void Renderer::create_screen_quad_primitives( uint32 width, uint32 height, rhi::CommandBuffer& cmdBuff ) {

		m_Ctx.m_RenderDevice( ).DestroyImage( m_ScreenQuad.m_FrameImage );

		rhi::Extent2D swapchainExtent = m_Ctx.m_RenderDevice( ).GetSwapchainExtent( );
		{
			rhi::ImageCreateInfo info{};
			info.m_ArrayLayers = 1;
			info.m_MipmapLevels = 1;
			info.m_Extent = { swapchainExtent.m_Width, swapchainExtent.m_Height, 1 };
			info.m_Dimension = rhi::ImageDimension::Image2D;
			info.m_Format = rhi::ImageFormat::RGBA8_SRGB;
			info.m_ViewType = rhi::ImageViewType::Default;
			info.m_UsageFlags = rhi::ImageUsageFlag::ShaderSampled | rhi::ImageUsageFlag::TransferDestination;

			m_ScreenQuad.m_FrameImage = m_Ctx.m_RenderDevice( ).CreateImage( info );
		}

	}

	void Renderer::setup_resources( rhi::CommandBuffer& cmdBuffer ) {

		{ // Frame sampler

			rhi::SamplerCreateInfo info{};
			m_ScreenQuad.m_FrameSampler = m_Ctx.m_RenderDevice( ).CreateSampler( info );

		}
		{ // VBO
			rhi::BufferCreateInfo info{};
			info.m_Usage = rhi::BufferUsageFlag::Vertex;
			info.m_BufferSize = m_ScreenQuad.s_Vertices.size( ) * sizeof( rhi::Vertex );
			info.m_DataSize = m_ScreenQuad.s_Vertices.size( ) * sizeof( rhi::Vertex );
			info.m_Data = m_ScreenQuad.s_Vertices.data( );
			m_ScreenQuad.m_VertexBuffer = m_Ctx.m_RenderDevice( ).CreateBuffer( info );
		}
		{ // EBO
			rhi::BufferCreateInfo info{};
			info.m_Usage = rhi::BufferUsageFlag::Element;
			info.m_BufferSize = m_ScreenQuad.s_Indices.size( ) * sizeof( uint32 );
			info.m_DataSize = m_ScreenQuad.s_Indices.size( ) * sizeof( uint32 );
			info.m_Data = m_ScreenQuad.s_Indices.data( );
			m_ScreenQuad.m_ElementBuffer = m_Ctx.m_RenderDevice( ).CreateBuffer( info );
		}
		{ // Resource set

			rhi::ResourceSetCreateInfo info{};
			info.m_Layout = m_ScreenQuad.m_ResourceLayout;

			m_ScreenQuad.m_ResourceSet = m_Ctx.m_RenderDevice( ).CreateResourceSet( info );

		}


		{ // Assign frame image and sampler to resource set
			rhi::UpdateImageInfo imgInfo{};
			imgInfo.m_ArrayIndex = 0;
			imgInfo.m_Binding = 0;
			imgInfo.m_ImageHandle = m_DeferredBuffer.GetAttachment( DeferredAttachment::Lit );
			imgInfo.m_SamplerHandle = m_ScreenQuad.m_FrameSampler;

			rhi::cmd::UpdateResourceSet setInfo{};
			setInfo.m_SetHandle = m_ScreenQuad.m_ResourceSet;
			setInfo.m_ImageInfos.PushBack( imgInfo );

			cmdBuffer.Push( setInfo );
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

			m_ScreenQuad.m_ResourceLayout = m_Ctx.m_RenderDevice( ).CreateResourceLayout( info );

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

			info.m_VertexInputPass = VertexLayoutConfiguration::GetStatic( ).GetDefaultConfiguration( );

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

			m_ScreenQuad.m_Pipeline = m_Ctx.m_RenderDevice( ).CreatePipeline( info );
		}

	}

} // namespace lum
