#pragma once
#include "eventmodule/details/engine_events/ev_ecs_events.hpp"
#include "entitymodule/details/ecs_define.hpp"
#include "entitymodule/details/ecs_component_pool.hpp"
struct Entity;

namespace ecs {

	class EntityManager {
	
	public:

		EntityManager()  { Init(); }
		~EntityManager() { Destruct(); }

		[[nodiscard]] Entity CreateEntity();

		template<detail::Component T>
		T* AddComponent(EntityID entityID);

		template<detail::Component T>
		void DeleteComponent(EntityID entityID);

		template<detail::Component T>
		T* GetComponent(EntityID entityID);

		template<detail::Component T>
		void RequireComponent(EntityID entityID);

		template<detail::Component T>
		bool Has(EntityID entityID);

	private:

		template<detail::Component T>
		detail::ComponentPool<T>& GetOrCreatePool();

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
#include "ecs_manager.ipp"