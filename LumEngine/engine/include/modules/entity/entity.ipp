//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Represents a unique entity in the scene.
//          Entity stores only an ID. ManagedEntity extends it
//          with component add/remove/get operations via MEntityManager.
//
//=============================================================================//
#pragma once
#include "entity/entity_manager.hpp"

namespace lum {

	template<ecs::detail::Component tType>
	tType& ManagedEntity::AddComponent( tType component ) {
		return mEntityManager->template AddComponent<tType>(mID, component);
	}

	template<ecs::detail::Component tType>
	tType* ManagedEntity::GetComponent( ) {
		return mEntityManager->template GetComponent<tType>(mID);
	}

	template<ecs::detail::Component tType>
	ManagedEntity& ManagedEntity::RemoveComponent( ) {
		mEntityManager->template RemoveComponent<tType>(mID);
		return *this;
	}

	template<ecs::detail::Component tType>
	bool ManagedEntity::HasComponent( ) {
		return mEntityManager->template HasComponent<tType>(mID);
	}

	template<ecs::detail::Component tType, ecs::detail::Component... tRest>
	bool ManagedEntity::HasComponents( ) {
		return mEntityManager->template HasComponents<tType, tRest...>(mID);
	}

}