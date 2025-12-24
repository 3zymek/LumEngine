#pragma once
#include "entity/ecs_common.hpp"
#include "event/ev_common.hpp"
namespace ev {
	struct ComponentAdded
	{
		LumEventTag;

		ecs::EntityID entityID{};
		ecs::detail::ComponentTypeID component_typeID{};
	};
	struct ComponentRemoved
	{
		LumEventTag;

		ecs::EntityID entityID{};
		ecs::detail::ComponentTypeID component_typeID{};
	};
}