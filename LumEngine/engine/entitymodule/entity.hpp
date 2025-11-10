#pragma once
#include "entitymodule/utils/ecs_define.hpp"
#include "ecs_manager.hpp"
abstract_class Entity {
public:

	force_inline const ecs::detail::EntityID& GetID() { return m_entityID; }

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

	ecs::detail::EntityID m_entityID = ecs::detail::GenerateEntityID::Get();

};