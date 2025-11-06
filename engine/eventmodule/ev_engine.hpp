#pragma once
#include "ev_base.hpp"
#include "e_define.hpp"
#include "ev_define.hpp"
#include "ecs_define.hpp"
namespace ev {
	struct EntityCreated : public ev::detail::BaseEvent
	{
		ecs::detail::EntityID entityID{};
		EntityCreated(ecs::detail::EntityID _id) : entityID(_id) {}
	};
	struct EntityDestroyed : public ev::detail::BaseEvent
	{
		ecs::detail::EntityID entityID{};
		EntityDestroyed(ecs::detail::EntityID _id) : entityID(_id) {}
	};
	struct EntityMove : public ev::detail::BaseEvent 
	{
		ecs::detail::EntityID entityID{};
		float offset{};
		EntityMove(ecs::detail::EntityID _id, float x) : entityID(_id), offset(x) {}
	};
	struct ComponentAdded : public ev::detail::BaseEvent
	{
		ecs::detail::EntityID entityID{};
		ComponentAdded(ecs::detail::EntityID _id) : entityID(_id) {}
	};
}