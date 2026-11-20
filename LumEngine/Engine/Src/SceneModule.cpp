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

	void SceneModule::Initialize(const SceneModuleCreateInfo& info) {

		SceneManagerContext ctx{};

		m_SceneMgr.Initialize(ctx);

	}

} // namespace lum