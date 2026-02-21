#include "entity/ecs_manager.hpp"

namespace lum::ecs {

	ManagedEntity MEntityManager::CreateEntity() {
		return ManagedEntity{ *this };
	}

	void MEntityManager::DestroyEntity(EntityID entityID) {

		for (EntityID i = 0; i < limits::gMaxComponents; i++) {
			if (mComponentPools[i] != nullptr)
				mComponentPools[i]->Remove(entityID);

		}
	}

	void MEntityManager::DestroyEntity(Entity entity) {
		for (EntityID i = 0; i < limits::gMaxComponents; i++) {

			if (mComponentPools[i] != nullptr)
				mComponentPools[i]->Remove(entity.mID);

		}
	}

}