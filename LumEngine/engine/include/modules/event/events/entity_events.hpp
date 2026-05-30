//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Entity and component lifecycle events.
//
//=============================================================================//
#pragma once
#include "event/event_common.hpp"
#include "entity/ecs_common.hpp"

namespace lum {

	struct EEntityCreated : public Event { EntityID mId; };
	struct EEntityDestroyed : public Event { EntityID mId; };

	// Fired when a component of type tType is added to an entity.
	template<ecs::detail::cComponent tType>
	struct EComponentAdded : public Event { EntityID mId; tType* mComponent; };

	// Fired when a component of type tType is removed from an entity.
	template<ecs::detail::cComponent tType>
	struct EComponentRemoved : public Event { EntityID mId; tType* mComponent; };

} // namespace lum