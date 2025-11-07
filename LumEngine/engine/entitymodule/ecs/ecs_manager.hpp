#pragma once
#include "ecs_define.hpp"
#include "ecs_component_pool.hpp"
#include <unordered_map>
#include <iostream>
namespace ecs {

	class EntityManager {

		EntityManager() {}
	
	public:

		static EntityManager& Global() {
			static EntityManager em;
			return em;
		}

		template<detail::Component T>
		force_inline T* AddComponent(detail::EntityID _id) {
			auto& pool = GetOrCreatePool<T>();
			return pool.Add(_id);
		}

		template<detail::Component T>
		force_inline void DeleteComponent(detail::EntityID _id) {
			auto& pool = GetOrCreatePool<T>();
			pool.Delete(_id);
		}

		template<detail::Component T>
		force_inline T* GetComponent(detail::EntityID _id) {
			auto& pool = GetOrCreatePool<T>();
			return pool.Get(_id);
		}

	private:

		template<detail::Component T>
		force_inline detail::ComponentPool<T>& GetOrCreatePool() {
			auto typeID = detail::EntityTypeIndex::get<T>();
			auto it = m_pools.find(typeID);
			if (it == m_pools.end()) {
				m_pools[typeID] = new detail::ComponentPool<T>;
				return *static_cast<detail::ComponentPool<T>*>(m_pools[typeID]);
			}
			return *static_cast<detail::ComponentPool<T>*>(it->second);
		}

		std::unordered_map<detail::EntityTypeID, BasePool*> m_pools;
		
	};

}