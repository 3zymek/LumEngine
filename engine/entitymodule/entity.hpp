#pragma once
#include "ecs_define.hpp"
#include "ecs_manager.hpp"
abstract_class Entity {
public:

	force_inline const ecs::detail::EntityID& GetID() { return m_entityID; }

	template<ecs::detail::Component T>
	force_inline std::optional<std::reference_wrapper<T>> AddComponent() {
		return ecs::EntityManager::Global().AddComponent<T>(m_entityID);
	}

	template<typename CompT>
	force_inline void DeleteComponent() {
		ecs::EntityManager::Global().DeleteComponent<CompT>(GetID());
	}
	
	template<typename CompT>
	force_inline std::optional<std::reference_wrapper<CompT>> GetComponent() {
		return ecs::EntityManager::Global().GetComponent<CompT>(GetID());
	}
	
private:

	ecs::detail::EntityID m_entityID = ecs::detail::GenerateEntityID::Get();

};