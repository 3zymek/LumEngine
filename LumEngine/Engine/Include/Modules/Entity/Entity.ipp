//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Represents a unique entity in the scene.
//          Entity stores only an ID. ManagedEntity extends it
//          with component add/remove/get operations via EntityManager.
//
//=============================================================================//
#pragma once
#include "Entity/EntityManager.hpp"

namespace lum {

	template<ecs::detail::cComponent tType>
	tType& ManagedEntity::AddComponent( tType component ) {
		return m_EntityManager().AddComponent<tType>(m_Id, component);
	}

	template<ecs::detail::cComponent tType>
	tType* ManagedEntity::GetComponent( ) {
		return m_EntityManager( ).GetComponent<tType>(m_Id);
	}

	template<ecs::detail::cComponent tType>
	ManagedEntity& ManagedEntity::RemoveComponent( ) {
		m_EntityManager( ).RemoveComponent<tType>(m_Id);
		return *this;
	}

	template<ecs::detail::cComponent tType>
	bool ManagedEntity::HasComponent( ) {
		return m_EntityManager( ).HasComponent<tType>(m_Id);
	}

	template<ecs::detail::cComponent tType, ecs::detail::cComponent... tRest>
	bool ManagedEntity::HasComponents( ) {
		return m_EntityManager( ).HasComponents<tType, tRest...>(m_Id);
	}

}