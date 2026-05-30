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

	ManagedEntity EntityManager::CreateEntity( ) {
		return ManagedEntity{ this };
	}

	void EntityManager::DestroyEntity( EntityID& entityID ) {

		for (EntityID i = 0; i < limits::kMaxComponentTypes; i++) {
			if (mComponentPools[ i ] != nullptr)
				mComponentPools[ i ]->Remove( entityID );

		}
	}

	void EntityManager::DestroyEntity( Entity& entity ) {
		for (EntityID i = 0; i < limits::kMaxComponentTypes; i++) {

			if (mComponentPools[ i ] != nullptr)
				mComponentPools[ i ]->Remove( entity.mId );

		}
	}



	//---------------------------------------------------------
	// Private
	//---------------------------------------------------------

	void EntityManager::initialize_pools( ) {
		for (int32 i = 0; i < limits::kMaxComponentTypes; i++) {
			mComponentPools[ i ] = nullptr;
		}
	}

	void EntityManager::destroy_pools( ) {
		for (int32 i = 0; i < limits::kMaxComponentTypes; i++) {
			if (mComponentPools[ i ] != nullptr) {
				delete mComponentPools[ i ];
				mComponentPools[ i ] = nullptr;
			}
		}
	}

}