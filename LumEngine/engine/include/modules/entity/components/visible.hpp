//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Render visibility component for the ECS.
//
//=============================================================================//
#pragma once
#include "entity/ecs_common.hpp"

namespace lum {

	/* @brief ECS component controlling the render visibility of an entity.
	*  Entities without this component or with bVisible set to false
	*  are skipped during the render pass.
	*/
	LCLASS( ) struct CVisibility : public Component {

		/* @brief Whether the entity should be submitted to the renderer this frame. */
		LPROPERTY( ) bool bVisible = true;

	};

} // namespace lum