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

// Header impl file
#include "RenderModule.hpp"

#include "Event/EventBus.hpp"
#include "PlatformModule.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/RenderingTypes.hpp"
#include "ResModule.hpp"

namespace lum {

	void RenderModule::Initialize( const RenderModuleCreateInfo& info ) {

		RendererContext ctx{};
		ctx.m_RenderDevice = info.m_RenderDevice;

		m_Renderer.Initialize( ctx, info.m_ViewportWidth, info.m_ViewportHeight );
		//m_RenderSys.Initialize( &m_Renderer );

	}

} // namespace lum