//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Manages entity component pools and their lifecycle.
//          Handles component add/remove/query per entity,
//          pool creation on demand, and multi-component iteration.
//
//=============================================================================//
#pragma once

#include "Entity/Entity.hpp"
#include "Entity/EntityManager.hpp"

#include "Event/EventBus.hpp"
#include "Event/Events/EntityEvents.hpp"

namespace lum::ecs {

	//-----------------------------------------------------------------------------
	// AddComponent
	//-----------------------------------------------------------------------------

	template<detail::cComponent tType>
	tType& EntityManager::AddComponent( EntityID entityId, tType component ) {
		tType& retVal = GetPool<tType>( ).Add( entityId, component );
		LUM_ASSERT( m_EventBus, "Event bus is nullptr" );
		m_EventBus().Emit( EComponentAdded<tType>{.m_Id = entityId, .m_Component = &retVal } );
		return retVal;
	}

	template<detail::cComponent tType>
	tType& EntityManager::AddComponent( Entity entity, tType component ) {
		tType& retVal = GetPool<tType>( ).Add( entity.m_Id, component );
		LUM_ASSERT( m_EventBus, "Event bus is nullptr" );
		m_EventBus( ).Emit( EComponentAdded<tType>{.m_Id = entity.m_Id, .m_Component = &retVal } );
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
		return GetPool<tType>( ).Get( entity.m_Id );
	}

	//-----------------------------------------------------------------------------
	// RemoveComponent
	//-----------------------------------------------------------------------------

	template<detail::cComponent tType>
	void EntityManager::RemoveComponent( EntityID entityId ) {
		GetPool<tType>( ).Remove( entityId );
		LUM_ASSERT( m_EventBus, "Event bus is nullptr" );
		m_EventBus( ).Emit( EComponentRemoved<tType>{ .m_Id = entityId } );
	}

	template<detail::cComponent tType>
	void EntityManager::RemoveComponent( Entity entity ) {
		GetPool<tType>( ).Remove( entity.m_Id );
		LUM_ASSERT( m_EventBus, "Event bus is nullptr" );
		m_EventBus( ).Emit( EComponentRemoved<tType>{ .m_Id = entity.m_Id } );
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
		return GetPool<tType>( ).Has( entity.m_Id );
	}

	template<detail::cComponent tFirst, detail::cComponent... tRest>
	bool EntityManager::HasComponents( EntityID entityId ) {
		return (HasComponent<tFirst>( entityId ) && (HasComponent<tRest>( entityId ) && ...));
	}

	template<detail::cComponent tFirst, detail::cComponent... tRest>
	bool EntityManager::HasComponents( Entity entity ) {
		return (HasComponent<tFirst>( entity.m_Id ) && (HasComponent<tRest>( entity.m_Id ) && ...));
	}

	//-----------------------------------------------------------------------------
	// Each
	//-----------------------------------------------------------------------------

	template<detail::cComponent tFirst, detail::cComponent... tRest, typename tCallback>
	void EntityManager::Each( tCallback&& callback ) {
		auto& pool = GetPool<tFirst>( );
		for (auto [id, component] : pool.m_Components.Each( )) {
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
		for (auto [id, component] : pool.m_Components.Each( )) {
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
		ComponentTypeID typeID = TypeRegistry::GetTypeId<tType>( );
		LUM_ASSERT( typeID < limits::k_MaxComponentTypes, "Too many component types" );
		if (m_ComponentPools[ typeID ] == nullptr)
			m_ComponentPools[ typeID ] = new detail::ComponentPool<tType>( );
		return *static_cast< detail::ComponentPool<tType>* >( m_ComponentPools[ typeID ] );
	}

} // namespace lum::ecs