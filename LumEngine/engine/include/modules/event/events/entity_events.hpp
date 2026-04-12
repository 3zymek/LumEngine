//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Entity and component lifecycle events.
//
//=============================================================================//
#pragma once
#include "event/event_common.hpp"
#include "entity/ecs_common.hpp"

namespace lum {

	struct EEntityCreated : public Event { EntityID mID; };
	struct EEntityDestroyed : public Event { EntityID mID; };

	// Fired when a component of type tType is added to an entity.
	template<ecs::detail::Component tType>
	struct EComponentAdded : public Event { EntityID mID; tType* mComponent; };

	// Fired when a component of type tType is removed from an entity.
	template<ecs::detail::Component tType>
	struct EComponentRemoved : public Event { EntityID mID; tType* mComponent; };

} // namespace lum