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
		m_Ctx.m_MeshMgr		= m_MeshMgr;
		m_Ctx.m_MaterialMgr	= m_MaterialMgr;
		m_Ctx.m_ShaderMgr		= m_ShaderMgr;
		m_Ctx.m_TextureMgr	= m_TextureMgr;
		m_Ctx.m_RenderDev		= platform.m_RenderDevice.get( );
		
		m_Ctx.Validate( );

		m_TextureMgr.Initialize( *platform.m_RenderDevice.get() );
		m_MeshMgr.Initialize( m_Ctx );
		m_ShaderMgr.Initialize( *platform.m_RenderDevice.get() );
		m_MaterialMgr.Initialize( m_Ctx );

	}

} // namespace lum