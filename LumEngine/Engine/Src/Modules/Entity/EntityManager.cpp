//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Manages entity component pools and their lifecycle.
//          Handles component add/remove/query per entity,
//          pool creation on demand, and multi-component iteration.
//
//=============================================================================//

#include "Entity/EntityManager.hpp"

namespace lum::ecs {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	ManagedEntity EntityManager::CreateEntity( ) {
		return ManagedEntity{ this };
	}

	void EntityManager::DestroyEntity( EntityID& entityID ) {

		for (EntityID i = 0; i < limits::k_MaxComponentTypes; i++) {
			if (m_ComponentPools[ i ] != nullptr)
				m_ComponentPools[ i ]->Remove( entityID );

		}
	}

	void EntityManager::DestroyEntity( Entity& entity ) {
		for (EntityID i = 0; i < limits::k_MaxComponentTypes; i++) {

			if (m_ComponentPools[ i ] != nullptr)
				m_ComponentPools[ i ]->Remove( entity.m_Id );

		}
	}



	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void EntityManager::initialize_pools( ) {
		for (int32 i = 0; i < limits::k_MaxComponentTypes; i++) {
			m_ComponentPools[ i ] = nullptr;
		}
	}

	void EntityManager::destroy_pools( ) {
		for (int32 i = 0; i < limits::k_MaxComponentTypes; i++) {
			if (m_ComponentPools[ i ] != nullptr) {
				delete m_ComponentPools[ i ];
				m_ComponentPools[ i ] = nullptr;
			}
		}
	}

}