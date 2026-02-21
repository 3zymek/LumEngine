#pragma once

#include "entity/entity.hpp"
#include "entity/ecs_manager.hpp"

template<lum::ecs::detail::Component tType>
tType& lum::ecs::MEntityManager::AddComponent(ecs::EntityID entityID, tType component) {
	return GetPool<tType>().Add(entityID, component);
}
template<lum::ecs::detail::Component tType>
tType& lum::ecs::MEntityManager::AddComponent(Entity entity, tType component) {
	return GetPool<tType>().Add(entity.mID, component);
}
template<lum::ecs::detail::Component tType>
tType* lum::ecs::MEntityManager::GetComponent(EntityID entityID) {
	return GetPool<tType>().Get(entityID);
}
template<lum::ecs::detail::Component tType>
tType* lum::ecs::MEntityManager::GetComponent(Entity entity) {
	return GetPool<tType>().Get(entity.mID);
}
template<lum::ecs::detail::Component tType>
void lum::ecs::MEntityManager::RemoveComponent(EntityID entityID) {
	GetPool<tType>().Remove(entityID);
}
template<lum::ecs::detail::Component tType>
void lum::ecs::MEntityManager::RemoveComponent(Entity entity) {
	GetPool<tType>().Remove(entity.mID);
}
template<lum::ecs::detail::Component tType>
bool lum::ecs::MEntityManager::HasComponent(EntityID entityID) {
	return GetPool<tType>().Has(entityID);
}
template<lum::ecs::detail::Component tType>
bool lum::ecs::MEntityManager::HasComponent(Entity entity) {
	return GetPool<tType>().Has(entity.mID);
}
template<lum::ecs::detail::Component tFirst, lum::ecs::detail::Component... tRest, typename tCallback>
void lum::ecs::MEntityManager::Each(tCallback&& callback) {

	auto& pool = GetPool<tFirst>();

	for (auto [id, component] : pool.mComponents.Each()) {

		bool hasAll = true;

		if constexpr (sizeof...(tRest) > 0)
			hasAll = (HasComponent<tRest>(id) && ...);

		if (hasAll) {
			callback(*component, *GetComponent<tRest>(id)...);
		}

	}

}
template<lum::ecs::detail::Component tFirst, lum::ecs::detail::Component... tRest, typename tCallback>
void lum::ecs::MEntityManager::EachWithID(tCallback&& callback) {

	auto& pool = GetPool<tFirst>();

	for (auto [id, component] : pool.mComponents.Each()) {

		bool hasAll = true;

		if constexpr (sizeof...(tRest) > 0)
			hasAll = (HasComponent<tRest>(id) && ...);

		if (hasAll) {
			callback(id, *component, *GetComponent<tRest>(id)...);
		}

	}

}
template<lum::ecs::detail::Component tType>
lum::ecs::detail::ComponentPool<tType>& lum::ecs::MEntityManager::GetPool( ) {

	uint32 typeID = GenerateTypeID::Get<tType>();
	if (mComponentPools[typeID] == nullptr)
		mComponentPools[typeID] = new detail::ComponentPool<tType>();

	return *static_cast<detail::ComponentPool<tType>*>(mComponentPools[typeID]);

}