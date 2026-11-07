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
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"

namespace lum {

	void GeometryPass::Initialize( RendererContext& ctx ) {

		ctx.Assert( );
		m_Ctx = ctx;

		m_PendingInstances.reserve( limits::k_MaxDrawCallsPerFrame );
		m_TemporaryInstances.reserve( s_TempSize );

		{
			rhi::BufferCreateInfo info{};
			info.m_Usage = rhi::BufferUsageFlag::Uniform;
			//info.m_
		}


	}

	void GeometryPass::SubmitInstance( const RenderInstance& instance ) {
		m_PendingInstances.push_back( instance );
	}

	void GeometryPass::DrawScene( ) {
		for (auto& instance : m_PendingInstances) {
			draw_instance( instance );
		}
	}

} // namespace lum