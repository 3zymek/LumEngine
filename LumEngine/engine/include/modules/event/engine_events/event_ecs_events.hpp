#pragma once
#include "entity/ecs_common.hpp"
#include "event/event_common.hpp"
namespace lum::ev {
	struct ComponentAdded
	{

		ecs::EntityID entityID;
		ecs::detail::ComponentTID component_typeID;
		LUM_EVENT_TAG;
	};
	struct ComponentRemoved
	{
		LUM_EVENT_TAG;

		ecs::EntityID entityID{};
		ecs::detail::ComponentTID component_typeID{};
	};
}