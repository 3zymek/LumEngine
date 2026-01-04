#pragma once
#include "event/engine_events/ev_ecs_events.hpp"
#include "entity/ecs_common.hpp"
#include "entity/ecs_component_pool.hpp"
#include "core/core_defines.hpp"
#include "event/ev_bus.hpp"
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

			[[nodiscard]] Entity CreateEntity();

			template<detail::Component T>
			T* AddComponent(EntityID entityID) {

				auto& pool = GetOrCreatePool<T>();

				event_bus.Emit<ev::ComponentAdded>({ entityID, detail::ComponentTypeID::Get<T>() });

				LOG_DEBUG(std::format("Added component {} to entity {}", typeid(T).name(), entityID));

				return pool.Add(entityID);

			}

			template<detail::Component T>
			void RemoveComponent(EntityID entityID) {

				auto& pool = GetOrCreatePool<T>();

				event_bus.Emit<ev::ComponentRemoved>({ entityID, detail::ComponentTypeID::Get<T>() });

				LOG_DEBUG(std::format("Removed component {} on entity {}", typeid(T).name(), entityID));

				pool.Delete(entityID);

			}

			template<detail::Component T>
			T* GetComponent(EntityID entityID) {

				auto& pool = GetOrCreatePool<T>();
				return pool.Get(entityID);

			}

			template<detail::Component T>
			void RequireComponent(EntityID entityID) {

				if (!Has<T>(entityID))
					AddComponent<T>(entityID);

				LOG_DEBUG(std::format("Required component {}", typeid(T).name()));

			}

			template<detail::Component T>
			bool Has(EntityID entityID) {
				auto& pool = GetOrCreatePool<T>();
				return pool.Has(entityID);
			}

		private:

			template<detail::Component T>
			detail::ComponentPool<T>& GetOrCreatePool() {

				auto typeID = detail::ComponentTypeID::Get<T>();

				if (!m_pools[typeID]) {
					m_pools[typeID] = new detail::ComponentPool<T>;
					return *static_cast<detail::ComponentPool<T>*>(m_pools[typeID]);
				}
				if (std::is_base_of_v<detail::UniqueComponent, T>)
					throw std::runtime_error("Unique component already exists");

				return *static_cast<detail::ComponentPool<T>*>(m_pools[typeID]);
			}

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

			detail::BasePool* m_pools[detail::MAX_COMPONENT_TYPES_COUNT];

		};

	}
}