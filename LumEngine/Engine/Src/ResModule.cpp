//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Resource module — GPU-side asset manager lifecycle.
//
//=============================================================================//

#include "ResModule.hpp"
#include "PlatformModule.hpp"

namespace lum {

//=======================================================//
	// Public
//=======================================================//

	void ResourceModule::Initialize( PlatformModule& platform, ev::EventBus& bus ) {

		m_Ctx.m_EventBus			= bus;
		m_Ctx.m_Meshm_Gr		= m_Meshm_Gr;
		m_Ctx.m_Materialm_Gr	= m_Materialm_Gr;
		m_Ctx.m_Shaderm_Gr		= m_Shaderm_Gr;
		m_Ctx.m_Texturem_Gr	= m_Texturem_Gr;
		m_Ctx.m_RenderDev		= platform.m_RenderDevice.get( );
		
		m_Ctx.Validate( );

		m_Texturem_Gr.Initialize( *platform.m_RenderDevice.get() );
		m_Meshm_Gr.Initialize( m_Ctx );
		m_Shaderm_Gr.Initialize( *platform.m_RenderDevice.get() );
		m_Materialm_Gr.Initialize( m_Ctx );

	}

} // namespace lum