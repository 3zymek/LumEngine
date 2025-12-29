#pragma once
#include "modules/entity/ecs_common.hpp"
#include "modules/event/ev_bus.hpp"
#include "modules/entity/ecs_component_pool.hpp"
namespace ecs {
	template<detail::Component T>
	T* EntityManager::AddComponent(EntityID entityID) {

		auto& pool = GetOrCreatePool<T>();

		//event

		return pool.Add(entityID);

	}

	template<detail::Component T>
	void EntityManager::DeleteComponent(EntityID entityID) {

		auto& pool = GetOrCreatePool<T>();

		//event

		pool.Delete(entityID);

	}

	template<detail::Component T>
	T* EntityManager::GetComponent(EntityID entityID) {

		auto& pool = GetOrCreatePool<T>();
		return pool.Get(entityID);

	}

	template<detail::Component T>
	void EntityManager::RequireComponent(EntityID entityID) {

		if (!Has<T>(entityID))
			AddComponent<T>(entityID);

	}

	template<detail::Component T>
	bool EntityManager::Has(EntityID entityID) {
		auto& pool = GetOrCreatePool<T>();
		return pool.Has(entityID);
	}

	template<detail::Component T>
	detail::ComponentPool<T>& EntityManager::GetOrCreatePool() {

		auto typeID = detail::ComponentTypeIndex::get<T>();

		if (!m_pools[typeID]) {
			m_pools[typeID] = new detail::ComponentPool<T>;
			return *static_cast<detail::ComponentPool<T>*>(m_pools[typeID]);
		}
		else if (std::is_base_of_v<detail::UniqueComponent, T>)
			throw detail::UniqueComponentAlreadyExists{ "unique component already exists" };

		return *static_cast<detail::ComponentPool<T>*>(m_pools[typeID]);
	}

}