#pragma once
#include "entitymodule/details/ecs_define.hpp"
namespace ecs { class EntityManager; }

abstract_class Entity {

	using EntityID = ecs::EntityID;
	using EntityManager = ecs::EntityManager;

	ecs::EntityManager& manager;

public:

	Entity( ecs::EntityManager& m, EntityID entityID ) : manager(m), m_entityID(entityID) {}
	~Entity() {}

	force_inline const EntityID& GetID() { return m_entityID; }

	template<ecs::detail::Component T>
	T* AddComponent();

	template<ecs::detail::Component T>
	void DeleteComponent();
	
	template<ecs::detail::Component T>
	T* GetComponent();

	template<ecs::detail::Component T>
	bool Has();
	
private:

	EntityID m_entityID{};

};
#include "entity.ipp"