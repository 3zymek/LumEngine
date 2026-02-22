//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Manages entity component pools and their lifecycle.
//          Handles component add/remove/query per entity,
//          pool creation on demand, and multi-component iteration.
//
//=============================================================================//

#include "entity/ecs_manager.hpp"

namespace lum::ecs {

	ManagedEntity MEntityManager::CreateEntity() {
		return ManagedEntity{ this };
	}

	void MEntityManager::DestroyEntity(EntityID entityID) {

		for (EntityID i = 0; i < limits::gMaxComponentTypes; i++) {
			if (mComponentPools[i] != nullptr)
				mComponentPools[i]->Remove(entityID);

		}
	}

	void MEntityManager::DestroyEntity(Entity entity) {
		for (EntityID i = 0; i < limits::gMaxComponentTypes; i++) {

			if (mComponentPools[i] != nullptr)
				mComponentPools[i]->Remove(entity.mID);

		}
	}

}