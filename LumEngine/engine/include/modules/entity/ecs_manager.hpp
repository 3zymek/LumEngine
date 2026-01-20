#pragma once
#include "event/engine_events/event_ecs_events.hpp"
#include "entity/ecs_common.hpp"
#include "entity/ecs_component_pool.hpp"
#include "core/core_defines.hpp"
#include "event/event_bus.hpp"
namespace lum {
	struct Entity;
	namespace ev { class EventBus; }
	namespace ecs {

		class EntityManager {

			// Context
			ev::EventBus& event_bus;

		public:

			EntityManager(ev::EventBus& bus) : event_bus(bus) { Init(); }
			~EntityManager() { Destruct(); }

			LUM_NODISCARD
			Entity CreateEntity();

			template<detail::Component T>
			T* AddComponent(EntityID entityID) {

				event_bus.Emit<ev::ComponentAdded>({ entityID, detail::ComponentTypeID::Get<T>() });

				LUM_LOG_DEBUG(std::format("Added component {} to entity {}", typeid(T).name(), entityID));

				return GetOrCreatePool<T>().Add(entityID);

			}

			template<detail::Component T>
			void RemoveComponent(EntityID entityID) {

				event_bus.Emit<ev::ComponentRemoved>({ entityID, detail::ComponentTypeID::Get<T>() });

				LUM_LOG_DEBUG(std::format("Removed component {} on entity {}", typeid(T).name(), entityID));

				GetOrCreatePool<T>().Delete(entityID);

			}

			template<detail::Component T>
			T* GetComponent(EntityID entityID) {

				return GetOrCreatePool<T>().Get(entityID);

			}

			template<detail::Component T>
			void RequireComponent(EntityID entityID) {

				if (!Has<T>(entityID))
					AddComponent<T>(entityID);

				LUM_LOG_DEBUG(std::format("Required component {}", typeid(T).name()));

			}

			template<detail::Component T>
			bool Has(EntityID entityID) {
				return GetOrCreatePool<T>().Has(entityID);
			}

		private:

			template<detail::Component T>
			detail::ComponentPool<T>& GetOrCreatePool() {

				auto typeID = detail::ComponentTypeID::Get<T>();

				if (!m_pools[typeID]) {
					m_pools[typeID] = new detail::ComponentPool<T>;
					return *static_cast<detail::ComponentPool<T>*>(m_pools[typeID]);
				}
				//static_assert(std::is_base_of_v<detail::UniqueComponent, T> && "Unique component already exists");

				return *static_cast<detail::ComponentPool<T>*>(m_pools[typeID]);
			}

			inline void Init() {
				for (usize i = 0; i < detail::MAX_COMPONENT_TYPES_COUNT; i++) {
					m_pools[i] = nullptr;
				}
				LUM_LOG_INIT_OK("Entity Component System init");
			}

			inline void Destruct() {
				for (usize i = 0; i < detail::MAX_COMPONENT_TYPES_COUNT; i++) {
					delete m_pools[i];
					m_pools[i] = nullptr;
				}
			}

			detail::BasePool* m_pools[detail::MAX_COMPONENT_TYPES_COUNT]{};

		};

	}
}