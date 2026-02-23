//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Material component for the ECS.
//
//=============================================================================//
#pragma once

#include "entity/ecs_common.hpp"
#include "render/material.hpp"

namespace lum {

	/* @brief ECS component holding a PBR material instance for an entity. */
	struct CMaterial : Component {

		FMaterialInstance mMat;

	};

} // namespace lum