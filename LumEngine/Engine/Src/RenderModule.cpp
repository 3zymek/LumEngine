//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Render module — renderer and render system lifecycle.
//
//=============================================================================//

#include "RenderModule.hpp"
#include "PlatformModule.hpp"
#include "ResModule.hpp"
#include "Event/EventBus.hpp"

namespace lum {

//=======================================================//
	// Public
//=======================================================//

	void RenderModule::Initialize( const RenderModuleCreateInfo& info ) {

		render::RendererContext ctx{};
		ctx.m_Materialm_Gr	= info.m_ResourceModule( ).m_Materialm_Gr;
		ctx.m_Meshm_Gr		= info.m_ResourceModule( ).m_Meshm_Gr;
		ctx.m_Texturem_Gr		= info.m_ResourceModule( ).m_Texturem_Gr;
		ctx.m_Shaderm_Gr		= info.m_ResourceModule( ).m_Shaderm_Gr;
		ctx.m_RenderDev		= info.m_RenderDev;
		ctx.m_EventBus		= info.m_EventBus;

		m_Renderer.Initialize( ctx, info.m_ViewportWidth, info.m_ViewportHeight );
		m_RenderSys.Initialize( &m_Renderer );

	}

} // namespace lum