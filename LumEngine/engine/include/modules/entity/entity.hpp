#pragma once
#include "entity/ecs_common.hpp"
#include "entity/ecs_manager.hpp"
#include "core/core_defines.hpp"
namespace lum {
	namespace ecs { class EntityManager; }

	class Entity {

		using EntityID = ecs::EntityID;
		using EntityManager = ecs::EntityManager;

		ecs::EntityManager& manager;

	public:

		Entity(ecs::EntityManager& m, EntityID entityID) : manager(m), m_entityID(entityID) {}
		~Entity( ) {}

		inline const EntityID& GetID( ) const noexcept { return m_entityID; }

		template< ecs::detail::Component T >
		T* AddComponent( ) {
			static_assert(std::is_trivially_copyable_v<T> && "Component is not trivially copyable");
			static_assert(std::is_trivially_destructible_v<T> && "Component is not trivially destructible");
			return manager.AddComponent<T>(m_entityID);
		}

		template< ecs::detail::Component T >
		void DeleteComponent( ) {
			manager.RemoveComponent<T>(m_entityID);
		}

		template< ecs::detail::Component T >
		T* GetComponent( ) {
			return manager.GetComponent<T>(m_entityID);
		}

		template< ecs::detail::Component T >
		LUMbool Has( ) {
			return manager.Has<T>(m_entityID);
		}

	private:

		EntityID m_entityID{};

	};
}