//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Represents a unique entity in the scene.
//          Entity stores only an ID. ManagedEntity extends it
//          with component add/remove/get operations via EntityManager.
//
//=============================================================================//
#pragma once
#include "entity/entity_manager.hpp"

namespace lum {

	template<ecs::detail::cComponent tType>
	tType& ManagedEntity::AddComponent( tType component ) {
		return mEntityManager->template AddComponent<tType>(mId, component);
	}

	template<ecs::detail::cComponent tType>
	tType* ManagedEntity::GetComponent( ) {
		return mEntityManager->template GetComponent<tType>(mId);
	}

	template<ecs::detail::cComponent tType>
	ManagedEntity& ManagedEntity::RemoveComponent( ) {
		mEntityManager->template RemoveComponent<tType>(mId);
		return *this;
	}

	template<ecs::detail::cComponent tType>
	bool ManagedEntity::HasComponent( ) {
		return mEntityManager->template HasComponent<tType>(mId);
	}

	template<ecs::detail::cComponent tType, ecs::detail::cComponent... tRest>
	bool ManagedEntity::HasComponents( ) {
		return mEntityManager->template HasComponents<tType, tRest...>(mId);
	}

}