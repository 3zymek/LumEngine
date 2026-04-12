//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Manages entity component pools and their lifecycle.
//          Handles component add/remove/query per entity,
//          pool creation on demand, and multi-component iteration.
//
//=============================================================================//

#include "entity/entity_manager.hpp"

namespace lum::ecs {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	ManagedEntity MEntityManager::CreateEntity( ) {
		return ManagedEntity{ this };
	}

	void MEntityManager::DestroyEntity( EntityID& entityID ) {

		for (EntityID i = 0; i < limits::gMaxComponentTypes; i++) {
			if (mComponentPools[ i ] != nullptr)
				mComponentPools[ i ]->Remove( entityID );

		}
	}

	void MEntityManager::DestroyEntity( Entity& entity ) {
		for (EntityID i = 0; i < limits::gMaxComponentTypes; i++) {

			if (mComponentPools[ i ] != nullptr)
				mComponentPools[ i ]->Remove( entity.mID );

		}
	}



	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void MEntityManager::init( ) {
		for (int32 i = 0; i < limits::gMaxComponentTypes; i++) {
			mComponentPools[ i ] = nullptr;
		}
	}

	void MEntityManager::destroy( ) {
		for (int32 i = 0; i < limits::gMaxComponentTypes; i++) {
			if (mComponentPools[ i ] != nullptr) {
				delete mComponentPools[ i ];
				mComponentPools[ i ] = nullptr;
			}
		}
	}

}