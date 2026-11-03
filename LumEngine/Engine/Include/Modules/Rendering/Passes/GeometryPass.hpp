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

#include "Core/CoreTypes.hpp"
#include "Core/Utils/SafePtr.hpp"
#include "Rendering/RenderingTypes.hpp"
#include "Rhi/Interfaces/Core/RhiBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"

namespace lum {

	struct CMaterialInstance;

	class GeometryPass {
	public:

		GeometryPass( ) = default;

		void Initialize( RendererContext& ctx );
		void SubmitInstance( const RenderInstance& instance );
		void DrawScene( );

	private:

		constexpr inline static usize s_TempSize = 64;

		std::vector<RenderInstance> m_PendingInstances{};
		std::vector<RenderInstance> m_TemporaryInstances{};

		rhi::ResourceLayoutHandle m_ResourceLayout{};
		rhi::ResourceSetHandle m_ResourceSet{};
		rhi::BufferHandle m_ModelUbo{};
		rhi::BufferHandle m_MaterialUbo{};
		rhi::PipelineHandle m_GeometryPipeline{};

		SafePtr<RendererContext> m_Ctx = nullptr;

		void draw_instance( const RenderInstance& instance ) noexcept;
		void draw_mesh( const RenderInstance& instance ) noexcept;
		void upload_model_matrix( const RenderInstance& instance ) noexcept;
		void upload_material( const CMaterialInstance& instance ) noexcept;

	};

} // namespace lum