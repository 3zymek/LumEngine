#pragma once

#include "event/event_common.hpp"
#include "entity/ecs_common.hpp"

namespace lum {

	struct EEntityCreated	: public Event { EntityID mID; };
	struct EEntityDestroyed : public Event { EntityID mID; };

	template<ecs::detail::Component tType>
	struct EComponentAdded : public Event { EntityID mID; };

	template<ecs::detail::Component tType>
	struct EComponentRemoved : public Event { EntityID mID; };

}