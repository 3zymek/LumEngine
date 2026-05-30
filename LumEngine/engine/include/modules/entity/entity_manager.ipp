//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Manages entity component pools and their lifecycle.
//          Handles component add/remove/query per entity,
//          pool creation on demand, and multi-component iteration.
//
//=============================================================================//
#pragma once
#include "entity/entity.hpp"
#include "entity/entity_manager.hpp"

#include "event/event_bus.hpp"
#include "event/events/entity_events.hpp"

namespace lum::ecs {

	//-----------------------------------------------------------------------------
	// AddComponent
	//-----------------------------------------------------------------------------

	template<detail::cComponent tType>
	tType& EntityManager::AddComponent( EntityID entityId, tType component ) {
		tType& retVal = GetPool<tType>( ).Add( entityId, component );
		LUM_ASSERT( mEventBus, "Event bus is nullptr" );
		mEventBus->Emit( EComponentAdded<tType>{.mId = entityId, .mComponent = &retVal } );
		return retVal;
	}

	template<detail::cComponent tType>
	tType& EntityManager::AddComponent( Entity entity, tType component ) {
		tType& retVal = GetPool<tType>( ).Add( entity.mId, component );
		LUM_ASSERT( mEventBus, "Event bus is nullptr" );
		mEventBus->Emit( EComponentAdded<tType>{.mId = entity.mId, .mComponent = &retVal } );
		return retVal;
	}

	//-----------------------------------------------------------------------------
	// GetComponent
	//-----------------------------------------------------------------------------

	template<detail::cComponent tType>
	tType* EntityManager::GetComponent( EntityID entityId ) {
		return GetPool<tType>( ).Get( entityId );
	}

	template<detail::cComponent tType>
	tType* EntityManager::GetComponent( Entity entity ) {
		return GetPool<tType>( ).Get( entity.mId );
	}

	//-----------------------------------------------------------------------------
	// RemoveComponent
	//-----------------------------------------------------------------------------

	template<detail::cComponent tType>
	void EntityManager::RemoveComponent( EntityID entityId ) {
		GetPool<tType>( ).Remove( entityId );
		LUM_ASSERT( mEventBus, "Event bus is nullptr" );
		mEventBus->Emit( EComponentRemoved<tType>{ .mId = entityId } );
	}

	template<detail::cComponent tType>
	void EntityManager::RemoveComponent( Entity entity ) {
		GetPool<tType>( ).Remove( entity.mId );
		LUM_ASSERT( mEventBus, "Event bus is nullptr" );
		mEventBus->Emit( EComponentRemoved<tType>{ .mId = entity.mId } );
	}

	//-----------------------------------------------------------------------------
	// HasComponent / HasComponents
	//-----------------------------------------------------------------------------

	template<detail::cComponent tType>
	bool EntityManager::HasComponent( EntityID entityId ) {
		return GetPool<tType>( ).Has( entityId );
	}

	template<detail::cComponent tType>
	bool EntityManager::HasComponent( Entity entity ) {
		return GetPool<tType>( ).Has( entity.mId );
	}

	template<detail::cComponent tFirst, detail::cComponent... tRest>
	bool EntityManager::HasComponents( EntityID entityId ) {
		return (HasComponent<tFirst>( entityId ) && (HasComponent<tRest>( entityId ) && ...));
	}

	template<detail::cComponent tFirst, detail::cComponent... tRest>
	bool EntityManager::HasComponents( Entity entity ) {
		return (HasComponent<tFirst>( entity.mId ) && (HasComponent<tRest>( entity.mId ) && ...));
	}

	//-----------------------------------------------------------------------------
	// Each
	//-----------------------------------------------------------------------------

	template<detail::cComponent tFirst, detail::cComponent... tRest, typename tCallback>
	void EntityManager::Each( tCallback&& callback ) {
		auto& pool = GetPool<tFirst>( );
		for (auto [id, component] : pool.mComponents.Each( )) {
			bool hasAll = true;
			if constexpr (sizeof...(tRest) > 0)
				hasAll = HasComponents<tFirst, tRest...>( id );
			if (hasAll)
				callback( *component, *GetComponent<tRest>( id )... );
		}
	}

	template<detail::cComponent tFirst, detail::cComponent... tRest, typename tCallback>
	void EntityManager::EachWithID( tCallback&& callback ) {
		auto& pool = GetPool<tFirst>( );
		for (auto [id, component] : pool.mComponents.Each( )) {
			bool hasAll = true;
			if constexpr (sizeof...(tRest) > 0)
				hasAll = HasComponents<tFirst, tRest...>( id );
			if (hasAll)
				callback( id, *component, *GetComponent<tRest>( id )... );
		}
	}

	//-----------------------------------------------------------------------------
	// GetPool
	//-----------------------------------------------------------------------------

	template<detail::cComponent tType>
	detail::ComponentPool<tType>& EntityManager::GetPool( ) {
		ComponentTypeID typeID = TypeRegistry::Get<tType>( );
		LUM_ASSERT( typeID < limits::kMaxComponentTypes, "Too many component types" );
		if (mComponentPools[ typeID ] == nullptr)
			mComponentPools[ typeID ] = new detail::ComponentPool<tType>( );
		return *static_cast< detail::ComponentPool<tType>* >( mComponentPools[ typeID ] );
	}

} // namespace lum::ecs