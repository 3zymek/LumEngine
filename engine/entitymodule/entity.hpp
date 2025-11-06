#pragma once
#include "ecs_define.hpp"
#include "ecs_manager.hpp"





ABSTRACT_CLASS Entity {
public:

	Entity(ecs::EntityManager* manager) : m_manager(manager) {}
	~Entity() = default;

	FORCEINLINE const ecs::detail::EntityID& GetID() { return m_entityID; }

	template<typename CompT>
	FORCEINLINE void AddComponent() {
		m_manager->AddComponent<CompT>(GetID());
	}

	template<typename CompT>
	FORCEINLINE void DeleteComponent() {
		m_manager->DeleteComponent<CompT>(GetID());
	}
	
	template<typename CompT>
	FORCEINLINE std::optional<std::reference_wrapper<CompT>> GetComponent() {
		return m_manager->GetComponent<CompT>(GetID());
	}

private:

	ecs::EntityManager* m_manager;

	ecs::detail::EntityID m_entityID;

};