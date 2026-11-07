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
#pragma once

#include <vector>

#include "Core/CoreDefines.hpp"
#include "Rendering/RenderingTypes.hpp"
#include "Rhi/Interfaces/Core/RhiBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiCommandBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiImage.hpp"
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"
#include "Rhi/Interfaces/Core/RhiResourceSet.hpp"
#include "Rhi/Interfaces/Core/RhiSampler.hpp"
#include "Rhi/RhiTypes.hpp"

namespace lum {

	class LUM_API Renderer {
	public:

		Renderer( ) = default;

		void Initialize( const RendererContext& ctx, uint32 initialWidth, uint32 initialHeight ) noexcept;
		const rhi::ImageHandle GetFrameTexture( ) const noexcept;
		void SubmitInstance( const RenderInstance& instance ) noexcept;

		void BeginFrame( );
		void EndFrame( );

	private:

		RendererContext m_Context{};

		rhi::CommandBuffer m_CmdBuffer{};

		struct {

			rhi::ImageHandle m_FrameImage{};
			rhi::SamplerHandle m_FrameSampler{};

			rhi::PipelineHandle m_Pipeline{};
			rhi::BufferHandle m_VertexBuffer{};
			rhi::BufferHandle m_ElementBuffer{};

			rhi::ResourceLayoutHandle m_ResourceLayout{};
			rhi::ResourceSetHandle m_ResourceSet{};

			static inline const std::vector<rhi::Vertex> s_Vertices = {
				// Bottom-left
				{ { -1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },

				// Bottom-right
				{ {  1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },

				// Top-right
				{ {  1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },

				// Top-left
				{ { -1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }
			};

			static inline const std::vector<uint32> s_Indices = {
				0, 1, 2,
				2, 3, 0
			};

		} m_ScreenQuad;

		void create_screen_quad_image( uint32 width, uint32 height, rhi::CommandBuffer& cmdBuff );
		void setup_resources( rhi::CommandBuffer& cmdBuffer );
		void create_main_pipeline( );

	};

} // namespace lum