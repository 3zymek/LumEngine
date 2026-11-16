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
#include "Rendering/Passes/GeometryPass.hpp"

#include "Core/Limits.hpp"
#include "Rendering/RenderingTypes.hpp"
#include "Rhi/Interfaces/Core/RhiBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiDevice.hpp"
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"
#include "Rhi/Interfaces/Core/RhiCommandBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiCommandTypes.hpp"
#include "Rendering/DeferredBuffer.hpp"
#include "Entity/Components/Mesh.hpp"
#include "Entity/Components/Material.hpp"
#include "Entity/Components/Transform.hpp"
#include "Rendering/VertexLayoutConfig.hpp"

namespace lum {

	void GeometryPass::Initialize( RendererContext& ctx, DeferredBuffer& deferredBuffer ) {

		ctx.Assert( );
		m_Ctx = ctx;

		m_MeshBuilder.Initialize( m_Ctx );

		{ // Material buffer
			rhi::BufferCreateInfo info{};
			info.m_Usage = rhi::BufferUsageFlag::Uniform;
			info.m_BufferSize = sizeof( MaterialGPU );
			m_MaterialUbo = m_Ctx( ).m_RenderDevice( ).CreateBuffer( info );
		}

		{
			rhi::BufferCreateInfo info{};
			info.m_Usage = rhi::BufferUsageFlag::Vertex;
			info.m_BufferSize = sizeof( rhi::Vertex );
		}

		{ // Geometry pipeline

			rhi::PipelineCreateInfo info{};

			rhi::PushConstantRange range{};
			range.m_Offset = 0;
			range.m_ShaderStageFlags = rhi::ShaderStageFlag::Vertex;
			range.m_Size = sizeof( Matrix4 );

			info.m_Layout.m_PushConstantRanges.push_back( range );

			// Albedo attachment
			rhi::ColorBlendAttachment albedo{};
			albedo.m_BlendEnabled = false;
			albedo.m_ColorMask = rhi::ColorComponentFlag::RGBA;

			info.m_ColorBlendPass.m_Attachments = {
				albedo
			};
			info.m_ColorAttachmentFormats = { rhi::ImageFormat::RGBA16_FLOAT };
			info.m_VertexInputPass = VertexLayoutConfiguration::GetStatic( ).GetDefaultConfiguration( );

			//auto& depth = info.m_DepthStencilPass;
			//depth.m_DepthTestEnabled = true;
			//depth.m_DepthWriteEnabled = true;

			auto& rast = info.m_RasterizationPass;
			rast.m_CullMode = rhi::CullMode::Back;
			rast.m_DepthBiasEnabled = false;
			rast.m_PolygonMode = rhi::PolygonMode::Fill;
			rast.m_FrontFace = rhi::FrontFace::Clockwise;
			rast.m_RasterizerEnabled = true;

			rhi::ShaderInfo vertexShader{};
			vertexShader.m_EntryPoint = "main";
			vertexShader.m_Path = ResourceLoader::ResolveResourcePath( ResourceRoot::Internal, "Shaders/geometry_pass.vert.spv" );
			vertexShader.m_Stage = rhi::ShaderStageFlag::Vertex;
			info.m_ShaderInfos.push_back( vertexShader );

			rhi::ShaderInfo fragmentShader{};
			fragmentShader.m_EntryPoint = "main";
			fragmentShader.m_Path = ResourceLoader::ResolveResourcePath( ResourceRoot::Internal, "Shaders/geometry_pass.frag.spv" );
			fragmentShader.m_Stage = rhi::ShaderStageFlag::Fragment;
			info.m_ShaderInfos.push_back( fragmentShader );

			info.m_AssemblyPass.m_Topology = rhi::PrimitiveTopology::TriangleList;

			m_GeometryPipeline = m_Ctx( ).m_RenderDevice( ).CreatePipeline( info );

		}

		m_AlbedoAttachment = deferredBuffer.GetAttachment( DeferredAttachment::Albedo );

	}

	void GeometryPass::SubmitInstance( const RenderInstance& instance ) {
		if (m_PendingInstances.Size( ) >= m_PendingInstances.Capacity( )) {
			LUM_LOG_WARN( "Max draw calls per frame limit reached" );
			return;
		}
		m_PendingInstances.PushBack( instance );
	}

	void GeometryPass::DrawScene( ) {

		rhi::CommandBuffer buffer{};

		rhi::RenderingAttachment colorAttachment{};
		colorAttachment.m_Image = m_AlbedoAttachment;
		colorAttachment.m_StoreOp = rhi::StoreOp::Store;
		colorAttachment.m_LoadOp = rhi::LoadOp::Clear;
		colorAttachment.m_ClearValue = colorAttachment.m_ClearValue.Color( { 0.0f, 0.0f, 0.0f, 0.0f } );

		rhi::RenderingAttachment depthAttachment{};
		depthAttachment.m_Image = m_DepthAttachment;
		depthAttachment.m_StoreOp = rhi::StoreOp::Store;
		depthAttachment.m_LoadOp = rhi::LoadOp::Clear;
		depthAttachment.m_ClearValue = rhi::RenderingAttachment::ClearValue::Color( { 0.0f, 0.0f, 0.0f, 0.0f } );

		rhi::cmd::BeginRendering beginRendering{};
		beginRendering.m_LayerCount = 1;
		beginRendering.m_ColorAttachments.PushBack( colorAttachment );
		beginRendering.m_RenderArea = m_Ctx( ).m_RenderDevice( ).GetSwapchainExtent( );

		rhi::cmd::TransitionImage albedoAsAttachment{};
		albedoAsAttachment.m_Image = m_AlbedoAttachment;
		albedoAsAttachment.m_NewUse = rhi::ImageUse::ColorAttachment;
		buffer.Push( albedoAsAttachment );

		buffer.Push( beginRendering );

		rhi::cmd::BindGraphicsPipeline bindPip{
			.m_Handle = m_GeometryPipeline
		};
		buffer.Push( bindPip );

		rhi::cmd::SetScissors setScissors{};
		rhi::cmd::SetViewport setViewport{};
		buffer.Push( setScissors );
		buffer.Push( setViewport );

		//rhi::cmd::BindResourceSet bindSet{
		//	.m_SetHandle = m_ResourceSet
		//};
		//buffer.Push( bindSet );


		for (auto& instance : m_PendingInstances) {
			draw_instance( instance, buffer );
		}
		m_PendingInstances.Clear( );

		buffer.Push( rhi::cmd::EndRendering{} );

		m_Ctx( ).m_RenderDevice( ).SendCommandBuffer( buffer );

	}

	void GeometryPass::draw_instance( const RenderInstance& instance, rhi::CommandBuffer& cmdBuffer ) noexcept {

		rhi::cmd::PushConstants modelConstant{
			.m_Data = instance.m_Transform( ).m_WorldMatrix.Data( ),
			.m_Size = sizeof( Matrix4 ),
			.m_Offset = 0,
			.m_ShaderStageFlags = rhi::ShaderStageFlag::Vertex
		};

		cmdBuffer.Push( modelConstant );

		//const auto mat = instance.m_Material;
		//upload_material( mat.Ref( ) );

		draw_mesh( instance, cmdBuffer );


	}

	void GeometryPass::draw_mesh( const RenderInstance& instance, rhi::CommandBuffer& cmdBuffer ) noexcept {

		const auto& renderResources = m_MeshBuilder.GetOrCreateMeshRenderResources( instance.m_StaticMesh( ).m_ResourceHandle );

		rhi::cmd::BindVertexBuffers bindVertexBuffers{};
		bindVertexBuffers.m_BufferHandles.PushBack( renderResources.m_VertexBuffer );
		bindVertexBuffers.m_FirstBinding = 0;
		cmdBuffer.Push( bindVertexBuffers );

		rhi::cmd::BindElementBuffer bindElementBuffer{};
		bindElementBuffer.m_BufferHandle = renderResources.m_ElementBuffer;
		bindElementBuffer.m_StartingOffset = 0;
		cmdBuffer.Push( bindElementBuffer );

		rhi::cmd::DrawIndexed drawIndexed{};
		drawIndexed.m_FirstIndex = 0;
		drawIndexed.m_FirstInstance = 0;
		drawIndexed.m_IndexCount = renderResources.m_IndicesCount;
		drawIndexed.m_InstanceCount = 1;
		drawIndexed.m_VertexOffset = 0;
		cmdBuffer.Push( drawIndexed );

	}
	void GeometryPass::upload_model_matrix( const RenderInstance& instance ) noexcept {



	}
	void GeometryPass::upload_material( const CMaterialInstance& instance ) noexcept {

		//MaterialGPU mat{};
		//mat.m_BaseColor = { instance.m_BaseColor, 1.0f };
		//mat.m_Metalness = instance.m_Metalness;
		//mat.m_Roughness = instance.m_Roughness;

		//rhi::cmd::

	}

} // namespace lum
