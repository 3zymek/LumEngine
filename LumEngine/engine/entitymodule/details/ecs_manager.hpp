#pragma once
#include "eventmodule/details/engine_events/ev_ecs_events.hpp"
#include "entitymodule/details/ecs_component_pool.hpp"
#include "entitymodule/details/ecs_define.hpp"
#include "eventmodule/details/ev_bus.hpp"
#include <unordered_map>
#include <iostream>
namespace ecs {

	class EntityManager {

		EntityManager()		{ Init(); }
		~EntityManager()	{ Destruct(); }
	
	public:

		static EntityManager& Global() {
			static EntityManager em;
			return em;
		}

		template<detail::Component T>
		force_inline T* AddComponent(EntityID _id) {

			auto& pool = GetOrCreatePool<T>();

			ev::EventBus::Engine().Emit<ev::ComponentAdded>(
				ev::ComponentAdded{ _id, detail::ComponentTypeIndex::get<T>() }
			);

			return pool.Add(_id);

		}

		template<detail::Component T>
		force_inline void DeleteComponent(EntityID _id) {

			auto& pool = GetOrCreatePool<T>();

			ev::EventBus::Engine().Emit<ev::ComponentRemoved>(
				ev::ComponentRemoved{ _id, detail::ComponentTypeIndex::get<T>() }
			);

			pool.Delete(_id);

		}

		template<detail::Component T>
		force_inline T* GetComponent(EntityID _id) {

			auto& pool = GetOrCreatePool<T>();
			return pool.Get(_id);

		}

	private:

		template<detail::Component T>
		force_inline detail::ComponentPool<T>& GetOrCreatePool() {

			auto typeID = detail::ComponentTypeIndex::get<T>();
			
			if (!m_pools[typeID]) {
				m_pools[typeID] = new detail::ComponentPool<T>;
				return *static_cast<detail::ComponentPool<T>*>(m_pools[typeID]);
			}
			else if (std::is_base_of_v<detail::UniqueComponent, T>)
				throw detail::UniqueComponentAlreadyExists { "unique component already exists" };

			return *static_cast<detail::ComponentPool<T>*>(m_pools[typeID]);
		}

		force_inline void Init() {
			for (size_t i = 0; i < detail::MAX_COMPONENT_TYPES_COUNT; i++) {
				m_pools[i] = nullptr;
			}
		}

		force_inline void Destruct() {
			for (size_t i = 0; i < detail::MAX_COMPONENT_TYPES_COUNT; i++) {
				delete m_pools[i];
				m_pools[i] = nullptr;
			}
		}


		BasePool* m_pools[detail::MAX_COMPONENT_TYPES_COUNT];
		
	};

}