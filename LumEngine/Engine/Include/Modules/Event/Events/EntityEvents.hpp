//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Entity and component lifecycle events.
//
//=============================================================================//
#pragma once

#include "Event/EventCommon.hpp"
#include "Entity/EcsCommon.hpp"

namespace lum {

	struct EEntityCreated : public Event { EntityID m_Id; };
	struct EEntityDestroyed : public Event { EntityID m_Id; };

	// Fired when a component of type tType is added to an entity.
	template<ecs::detail::cComponent tType>
	struct EComponentAdded : public Event { 
		EntityID m_Id{};
		tType* m_Component = nullptr; 
	};

	// Fired when a component of type tType is removed from an entity.
	template<ecs::detail::cComponent tType>
	struct EComponentRemoved : public Event {
		EntityID m_Id{};
		tType* m_Component = nullptr;
	};

} // namespace lum