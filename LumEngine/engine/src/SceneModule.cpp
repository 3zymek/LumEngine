//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Scene module — scene manager lifecycle.
//
//=============================================================================//

#include "SceneModule.hpp"
#include "RenderModule.hpp"
#include "ResModule.hpp"
#include "AudioModule.hpp"
#include "Event/EventBus.hpp"

namespace lum {

	void SceneModule::Initialize( const SceneModuleCreateInfo& info ) {

		SceneManagerContext ctx;
		ctx.m_Audiom_Gr = info.m_AudioModule( ).m_Audiom_Gr;
		ctx.m_Materialm_Gr = info.m_ResourceModule( ).m_Materialm_Gr;
		ctx.m_Meshm_Gr = info.m_ResourceModule( ).m_Meshm_Gr;
		ctx.m_Texturem_Gr = info.m_ResourceModule( ).m_Texturem_Gr;
		ctx.m_Shaderm_Gr = info.m_ResourceModule( ).m_Shaderm_Gr;
		ctx.m_Renderer = info.m_RenderModule( ).m_Renderer;
		ctx.m_EventBus = info.m_EventBus;

		m_Scenem_Gr.Initialize( ctx );

	}

} // namespace lum