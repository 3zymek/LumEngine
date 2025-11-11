#pragma once
#include "entitymodule/details/ecs_define.hpp"
#include "entitymodule/details/ecs_manager.hpp"
abstract_class Entity {
public:

	force_inline const ecs::EntityID& GetID() { return m_entityID; }

	template<ecs::detail::Component T>
	force_inline T* AddComponent() {
		return ecs::EntityManager::Global().AddComponent<T>(m_entityID);
	}

	template<ecs::detail::Component T>
	force_inline void DeleteComponent() {
		ecs::EntityManager::Global().DeleteComponent<T>(GetID());
	}
	
	template<ecs::detail::Component T>
	force_inline T* GetComponent() {
		return ecs::EntityManager::Global().GetComponent<T>(GetID());
	}
	
private:

	ecs::EntityID m_entityID = ecs::detail::GenerateEntityID::Get();

};