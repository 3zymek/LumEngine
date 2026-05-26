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
		return mEntityManager->template AddComponent<tType>(mId, component);
	}

	template<ecs::detail::Component tType>
	tType* ManagedEntity::GetComponent( ) {
		return mEntityManager->template GetComponent<tType>(mId);
	}

	template<ecs::detail::Component tType>
	ManagedEntity& ManagedEntity::RemoveComponent( ) {
		mEntityManager->template RemoveComponent<tType>(mId);
		return *this;
	}

	template<ecs::detail::Component tType>
	bool ManagedEntity::HasComponent( ) {
		return mEntityManager->template HasComponent<tType>(mId);
	}

	template<ecs::detail::Component tType, ecs::detail::Component... tRest>
	bool ManagedEntity::HasComponents( ) {
		return mEntityManager->template HasComponents<tType, tRest...>(mId);
	}

}