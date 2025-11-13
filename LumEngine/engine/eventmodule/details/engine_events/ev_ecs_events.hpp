#pragma once
#include "entitymodule/details/ecs_define.hpp"
#include "eventmodule/details/ev_define.hpp"
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