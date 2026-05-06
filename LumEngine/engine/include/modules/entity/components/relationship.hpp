//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Parent and child relationship components for the ECS hierarchy.
//
//=============================================================================//
#pragma once

#include "entity/ecs_common.hpp"

namespace lum {

	/* @brief ECS component marking an entity as a child of another entity. */
	LCLASS() struct CChild : public Component {

		/* @brief ID of the parent entity. */
		LPROPERTY() int mParentID;

	};

	/* @brief ECS component marking an entity as a parent of another entity. */
	LCLASS() struct CParent : public Component {

		/* @brief ID of the child entity. */
		EntityID mChildID;

	};

} // namespace lum