//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Entity template implementation
//
//=============================================================================//
#pragma once

#include "entity/ecs_manager.hpp"

template<lum::ecs::detail::Component tType>
tType& lum::ManagedEntity::AddComponent(tType component) {
	return mEntityManager.template AddComponent<tType>(mID, component);
}
template<lum::ecs::detail::Component tType>
tType* lum::ManagedEntity::GetComponent() {
	return mEntityManager.template GetComponent<tType>(mID);
}
template<lum::ecs::detail::Component tType>
lum::ManagedEntity& lum::ManagedEntity::RemoveComponent() {
	mEntityManager.template RemoveComponent<tType>(mID);
	return *this;
}
template<lum::ecs::detail::Component tType>
bool lum::ManagedEntity::HasComponent(tType component) {
	mEntityManager.template HasComponent<tType>(mID);
}