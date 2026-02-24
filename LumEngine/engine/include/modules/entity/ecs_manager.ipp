//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Manages entity component pools and their lifecycle.
//          Handles component add/remove/query per entity,
//          pool creation on demand, and multi-component iteration.
//
//=============================================================================//
#pragma once

#include "entity/entity.hpp"
#include "entity/ecs_manager.hpp"

namespace lum::ecs {

	//-----------------------------------------------------------------------------
	// AddComponent
	//-----------------------------------------------------------------------------

	template<detail::Component tType>
	tType& MEntityManager::AddComponent( EntityID entityID, tType component ) {
		LUM_LOG_DEBUG("Added component %s to entity %d", typeid(tType).name(), entityID);
		return GetPool<tType>().Add(entityID, component);
	}

	template<detail::Component tType>
	tType& MEntityManager::AddComponent( Entity entity, tType component ) {
		LUM_LOG_DEBUG("Added component %s to entity %d", typeid(tType).name(), entity.mID);
		return GetPool<tType>().Add(entity.mID, component);
	}

	//-----------------------------------------------------------------------------
	// GetComponent
	//-----------------------------------------------------------------------------

	template<detail::Component tType>
	tType* MEntityManager::GetComponent( EntityID entityID ) {
		return GetPool<tType>().Get(entityID);
	}

	template<detail::Component tType>
	tType* MEntityManager::GetComponent( Entity entity ) {
		return GetPool<tType>().Get(entity.mID);
	}

	//-----------------------------------------------------------------------------
	// RemoveComponent
	//-----------------------------------------------------------------------------

	template<detail::Component tType>
	void MEntityManager::RemoveComponent( EntityID entityID ) {
		GetPool<tType>().Remove(entityID);
	}

	template<detail::Component tType>
	void MEntityManager::RemoveComponent( Entity entity ) {
		GetPool<tType>().Remove(entity.mID);
	}

	//-----------------------------------------------------------------------------
	// HasComponent / HasComponents
	//-----------------------------------------------------------------------------

	template<detail::Component tType>
	bool MEntityManager::HasComponent( EntityID entityID ) {
		return GetPool<tType>().Has(entityID);
	}

	template<detail::Component tType>
	bool MEntityManager::HasComponent( Entity entity ) {
		return GetPool<tType>().Has(entity.mID);
	}

	template<detail::Component tFirst, detail::Component... tRest>
	bool MEntityManager::HasComponents( EntityID entityID ) {
		return (HasComponent<tFirst>(entityID) && (HasComponent<tRest>(entityID) && ...));
	}

	template<detail::Component tFirst, detail::Component... tRest>
	bool MEntityManager::HasComponents( Entity entity ) {
		return (HasComponent<tFirst>(entity.mID) && (HasComponent<tRest>(entity.mID) && ...));
	}

	//-----------------------------------------------------------------------------
	// Each
	//-----------------------------------------------------------------------------

	template<detail::Component tFirst, detail::Component... tRest, typename tCallback>
	void MEntityManager::Each( tCallback&& callback ) {
		auto& pool = GetPool<tFirst>();
		for (auto [id, component] : pool.mComponents.Each()) {
			bool hasAll = true;
			if constexpr (sizeof...(tRest) > 0)
				hasAll = HasComponents<tFirst, tRest...>(id);
			if (hasAll)
				callback(*component, *GetComponent<tRest>(id)...);
		}
	}

	template<detail::Component tFirst, detail::Component... tRest, typename tCallback>
	void MEntityManager::EachWithID( tCallback&& callback ) {
		auto& pool = GetPool<tFirst>();
		for (auto [id, component] : pool.mComponents.Each()) {
			bool hasAll = true;
			if constexpr (sizeof...(tRest) > 0)
				hasAll = HasComponents<tFirst, tRest...>(id);
			if (hasAll)
				callback(id, *component, *GetComponent<tRest>(id)...);
		}
	}

	//-----------------------------------------------------------------------------
	// GetPool
	//-----------------------------------------------------------------------------

	template<detail::Component tType>
	detail::ComponentPool<tType>& MEntityManager::GetPool( ) {
		uint32 typeID = GenerateTypeID::Get<tType>();
		LUM_ASSERT(typeID < limits::gMaxComponentTypes, "Too many component types");
		if (mComponentPools[typeID] == nullptr)
			mComponentPools[typeID] = new detail::ComponentPool<tType>();
		return *static_cast<detail::ComponentPool<tType>*>(mComponentPools[typeID]);
	}

} // namespace lum::ecs