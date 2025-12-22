#pragma once
#include "include/modules/event/engine_events/ev_ecs_events.hpp"
#include "include/modules/entity/ecs_common.hpp"
#include "include/modules/entity/ecs_component_pool.hpp"
#include "include/core/core_defines.hpp"
struct Entity;

namespace ecs {

	class EntityManager {

	public:

		EntityManager() { Init(); }
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

		inline void Init() {
			for (size_t i = 0; i < detail::MAX_COMPONENT_TYPES_COUNT; i++) {
				m_pools[i] = nullptr;
			}
			LOG_INIT_OK("Entity Component System init");
		}

		inline void Destruct() {
			for (size_t i = 0; i < detail::MAX_COMPONENT_TYPES_COUNT; i++) {
				delete m_pools[i];
				m_pools[i] = nullptr;
			}
		}


		BasePool* m_pools[detail::MAX_COMPONENT_TYPES_COUNT];

	};

}
#include "ecs_manager.ipp"