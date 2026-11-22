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

// Header impl file
#include "Rendering/Passes/LightPass.hpp"

#include "Core/Utils/SafePtr.hpp"
#include "Core/Utils/ResourceLoader.hpp"
#include "Core/Math/MatrixOperations.hpp"
#include "Core/Math/MathUtils.hpp"
#include "Rhi/Interfaces/Core/RhiDevice.hpp"
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"
#include "Rhi/Interfaces/Core/RhiCommandTypes.hpp"
#include "Rendering/DeferredBuffer.hpp"
#include "Rendering/VertexLayoutConfig.hpp"

namespace lum {

	void LightPass::Initialize( RendererContext& ctx, DeferredBuffer& buffer ) {

		ctx.Assert( );
		m_Ctx = ctx;

		m_DeferredBuffer = buffer;

		{ // Resource layout

			rhi::ResourceLayoutCreateInfo info{};

			rhi::ResourceSetBinding cameraBinding{};
			cameraBinding.m_Binding = 0;
			cameraBinding.m_ResourceCount = 1;
			cameraBinding.m_ShaderStages = rhi::ShaderStageFlag::Fragment;
			cameraBinding.m_Type = rhi::ResourceType::UniformBuffer;
			info.m_Bindings.push_back( cameraBinding );

			rhi::ResourceSetBinding albedoBinding{};
			albedoBinding.m_Binding = 1;
			albedoBinding.m_ResourceCount = 1;
			albedoBinding.m_ShaderStages = rhi::ShaderStageFlag::Fragment;
			albedoBinding.m_Type = rhi::ResourceType::CombinedImageSampler;
			info.m_Bindings.push_back( albedoBinding );

			rhi::ResourceSetBinding depthBinding{};
			depthBinding.m_Binding = 2;
			depthBinding.m_ResourceCount = 1;
			depthBinding.m_ShaderStages = rhi::ShaderStageFlag::Fragment;
			depthBinding.m_Type = rhi::ResourceType::CombinedImageSampler;
			info.m_Bindings.push_back( depthBinding );

			rhi::ResourceSetBinding dirLightBinding{};
			dirLightBinding.m_Binding = 3;
			dirLightBinding.m_ResourceCount = 1;
			dirLightBinding.m_ShaderStages = rhi::ShaderStageFlag::Fragment;
			dirLightBinding.m_Type = rhi::ResourceType::UniformBuffer;
			info.m_Bindings.push_back( dirLightBinding );

			m_ResourceLayout = m_Ctx( ).m_RenderDevice( ).CreateResourceLayout( info );

		}
		{ // Lights pipeline

			rhi::PipelineLayout layout{};
			layout.m_ResourceLayouts.push_back( m_ResourceLayout );

			rhi::PipelineCreateInfo info{};
			info.m_Layout = layout;

			rhi::ShaderInfo vertexInfo{};
			vertexInfo.m_EntryPoint = "main";
			vertexInfo.m_Path = ResourceLoader::ResolveResourcePath(
				ResourceRoot::Internal, "Shaders/LightPass.vert.spv"
			);
			vertexInfo.m_Stage = rhi::ShaderStageFlag::Vertex;
			info.m_ShaderInfos.push_back( vertexInfo );

			rhi::ShaderInfo fragmentInfo{};
			fragmentInfo.m_EntryPoint = "main";
			fragmentInfo.m_Path = ResourceLoader::ResolveResourcePath(
				ResourceRoot::Internal, "Shaders/LightPass.frag.spv"
			);
			fragmentInfo.m_Stage = rhi::ShaderStageFlag::Fragment;
			info.m_ShaderInfos.push_back( fragmentInfo );

			info.m_ColorAttachmentFormats = { rhi::ImageFormat::RGBA16_FLOAT };
			rhi::ColorBlendAttachment litBlend{};
			litBlend.m_BlendEnabled = false;
			litBlend.m_ColorMask = rhi::ColorComponentFlag::RGBA;
			info.m_ColorBlendPass.m_Attachments = { litBlend };
			info.m_VertexInputPass = {};
			info.m_RasterizationPass.m_CullMode = rhi::CullMode::None;
			info.m_DepthStencilPass.m_DepthTestEnabled = false;
			info.m_DepthStencilPass.m_DepthWriteEnabled = false;

			m_LightsPipeline = m_Ctx( ).m_RenderDevice( ).CreatePipeline( info );

		}

		{ // Resource set

			rhi::ResourceSetCreateInfo info{};
			info.m_Layout = m_ResourceLayout;
			m_ResourceSet = m_Ctx( ).m_RenderDevice( ).CreateResourceSet( info );

		}

		{ // Directional light buffer

			rhi::BufferCreateInfo info{};
			info.m_BufferSize = sizeof( DirectionalLight );
			info.m_Usage = rhi::BufferUsageFlag::Uniform | rhi::BufferUsageFlag::TransferDestination;
			m_DirLightUbo = m_Ctx( ).m_RenderDevice( ).CreateBuffer( info );

		}
		{
			rhi::BufferCreateInfo info{};
			info.m_BufferSize = sizeof( m_Camera );
			info.m_Usage = rhi::BufferUsageFlag::Uniform | rhi::BufferUsageFlag::TransferDestination;
			m_CameraUbo = m_Ctx( ).m_RenderDevice( ).CreateBuffer( info );
		}
		{
			rhi::SamplerCreateInfo info{};
			info.m_MagFilter = rhi::SamplerFilter::Nearest;
			info.m_MinFilter = rhi::SamplerFilter::Nearest;
			info.m_MipmapFilter = rhi::SamplerFilter::Nearest;
			info.m_WrapU = rhi::SamplerWrap::ClampEdge;
			info.m_WrapV = rhi::SamplerWrap::ClampEdge;
			info.m_WrapW = rhi::SamplerWrap::ClampEdge;
			info.m_MaxLod = 0.0f;
			m_FrameSampler = m_Ctx( ).m_RenderDevice( ).CreateSampler( info );
		}

		RefreshResources( );

	}

	void LightPass::RefreshResources( ) {
		rhi::CommandBuffer cmdBuffer{};
		update_resource_set( cmdBuffer );
		m_Ctx( ).m_RenderDevice( ).SendCommandBuffer( cmdBuffer );
	}

	void LightPass::DrawScene( ) {

		rhi::CommandBuffer cmdBuffer{};

		rhi::cmd::UpdateBuffer cameraUpdate{};
		cameraUpdate.m_Buffer = m_CameraUbo;
		cameraUpdate.m_NewData = &m_Camera;
		cameraUpdate.m_DataSize = sizeof( m_Camera );
		cmdBuffer.Push( cameraUpdate );

		rhi::cmd::UpdateBuffer lightUpdate{};
		lightUpdate.m_Buffer = m_DirLightUbo;
		lightUpdate.m_NewData = &m_DirLight;
		lightUpdate.m_DataSize = sizeof( m_DirLight );
		cmdBuffer.Push( lightUpdate );

		for (const auto attachment : { DeferredAttachment::Albedo, DeferredAttachment::Depth }) {
			rhi::cmd::TransitionImage sampled{};
			sampled.m_Image = m_DeferredBuffer( ).GetAttachment( attachment );
			sampled.m_NewUse = rhi::ImageUse::Sampled;
			sampled.m_ShaderStages = rhi::ShaderStageFlag::Fragment;
			cmdBuffer.Push( sampled );
		}

		rhi::RenderingAttachment litAttachment{};
		litAttachment.m_Image = m_DeferredBuffer( ).GetAttachment( DeferredAttachment::Lit );
		litAttachment.m_StoreOp = rhi::StoreOp::Store;
		litAttachment.m_LoadOp = rhi::LoadOp::Clear;
		litAttachment.m_ClearValue = litAttachment.m_ClearValue.Color( { 0.0f, 0.0f, 0.0f, 0.0f } );

		rhi::cmd::BeginRendering beginRendering{};
		beginRendering.m_LayerCount = 1;
		beginRendering.m_ColorAttachments.PushBack( litAttachment );
		beginRendering.m_RenderArea = m_Ctx( ).m_RenderDevice( ).GetSwapchainExtent( );

		rhi::cmd::TransitionImage litAsAttachment{};
		litAsAttachment.m_Image = m_DeferredBuffer( ).GetAttachment( DeferredAttachment::Lit );
		litAsAttachment.m_NewUse = rhi::ImageUse::ColorAttachment;
		cmdBuffer.Push( litAsAttachment );

		cmdBuffer.Push( beginRendering );

		rhi::cmd::BindGraphicsPipeline bindPip{
			.m_Handle = m_LightsPipeline
		};
		cmdBuffer.Push( bindPip );

		rhi::cmd::BindResourceSet bindSet{
			.m_SetHandle = m_ResourceSet
		};

		cmdBuffer.Push( bindSet );
		cmdBuffer.Push( rhi::cmd::SetScissors{} );
		cmdBuffer.Push( rhi::cmd::SetViewport{} );

		rhi::cmd::Draw draw{
			.m_VertexCount = 3,
			.m_InstanceCount = 1,
			.m_FirstVertex = 0,
			.m_FirstInstance = 0
		};
		cmdBuffer.Push( draw );

		cmdBuffer.Push( rhi::cmd::EndRendering{} );

		m_Ctx( ).m_RenderDevice( ).SendCommandBuffer( cmdBuffer );

	}

	void LightPass::SetDirectionalLight( const DirectionalLight& light ) noexcept {
		m_DirLight = light;
	}

	void LightPass::SubmitCameraData( const CameraData& data ) {
		m_Camera.m_View = LookAt( data.m_Position, data.m_Position + data.m_Front, data.m_Up );
		const auto extent = m_Ctx( ).m_RenderDevice( ).GetSwapchainExtent( );
		m_Camera.m_Projection = Perspective( Radians( data.m_Fov ), static_cast<float64>(extent.m_Width) / static_cast<float64>(extent.m_Height), 0.01f, 10000.0f );
		m_Camera.m_InvertedProjection = Inverse( m_Camera.m_Projection * m_Camera.m_View );
		m_Camera.m_Position = Vector4( data.m_Position.m_X, data.m_Position.m_Y, data.m_Position.m_Z, 0.0f );
	}

	void LightPass::update_resource_set( rhi::CommandBuffer& cmdBuffer ) noexcept {
		rhi::cmd::UpdateResourceSet update{};
		update.m_SetHandle = m_ResourceSet;

		for (const auto binding : { 1U, 2U }) {
			rhi::UpdateImageInfo image{};
			image.m_ImageHandle = m_DeferredBuffer( ).GetAttachment( binding == 1 ? DeferredAttachment::Albedo : DeferredAttachment::Depth );
			image.m_SamplerHandle = m_FrameSampler;
			image.m_Binding = binding;
			update.m_ImageInfos.PushBack( image );
		}

		rhi::UpdateBufferInfo camera{};
		camera.m_BufferHandle = m_CameraUbo;
		camera.m_Range = sizeof( m_Camera );
		camera.m_Binding = 0;
		update.m_BufferInfos.PushBack( camera );

		rhi::UpdateBufferInfo light{};
		light.m_BufferHandle = m_DirLightUbo;
		light.m_Range = sizeof( m_DirLight );
		light.m_Binding = 3;
		update.m_BufferInfos.PushBack( light );
		cmdBuffer.Push( update );
	}

}
