#pragma once
#include "entity/ecs_common.hpp"
#include "event/event_common.hpp"
namespace lum::ev {
	struct ComponentAdded
	{

		ecs::EntityID entityID;
		ecs::detail::ComponentTID component_typeID;
		LumEventTag;
	};
	struct ComponentRemoved
	{
		LumEventTag;

		ecs::EntityID entityID{};
		ecs::detail::ComponentTID component_typeID{};
	};
}