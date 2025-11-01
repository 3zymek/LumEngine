#pragma once
#include "ecs_define.hpp"
#include "ecs_component_pool.hpp"
#include <unordered_map>

namespace ecs {

	class EntityManager {
	public:

		template<typename CompT>
		FORCEINLINE void AddComponent(detail::EntityID _id) {
			ECS_ASSERT_IS_COMPONENT(CompT);
			ENGINE_LOG("comp added");
			auto& pool = GetOrCreatePool<CompT>();
			pool.Add(_id);
		}

		template<typename CompT>
		FORCEINLINE void DeleteComponent(detail::EntityID _id) {
			ECS_ASSERT_IS_COMPONENT(CompT);
			auto& pool = GetOrCreatePool<CompT>();
			pool.Delete(_id);
		}

		template<typename CompT>
		FORCEINLINE std::optional<std::reference_wrapper<CompT>> GetComponent(detail::EntityID _id) {
			ECS_ASSERT_IS_COMPONENT(CompT);
			auto& pool = GetOrCreatePool<CompT>();
			return pool.Get(_id);
		}

	private:

		template<typename CompT>
		FORCEINLINE detail::ComponentPool<CompT>& GetOrCreatePool() {
			auto typeID = detail::EntityTypeIndex::get<CompT>();
			auto it = m_pools.find(typeID);
			if (it == m_pools.end()) {
				m_pools[typeID] = new detail::ComponentPool<CompT>;
				return *static_cast<detail::ComponentPool<CompT>*>(m_pools[typeID]);
			}
			return *static_cast<detail::ComponentPool<CompT>*>(it->second);
		}

		std::unordered_map<detail::EntityTypeID, detail::BasePool*> m_pools;
		
	};

}