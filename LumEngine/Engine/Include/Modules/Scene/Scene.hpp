//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Represents a single scene.
// 
//=============================================================================//
#pragma once

#include "Entity/EntityManager.hpp"
#include "Core/Utils/Optional.hpp"

namespace lum {

    /* @brief Represents a single scene — holds a list of active entities
    *  and their corresponding ECS manager.
    */
    class LUM_API Scene {
    public:

        /* @brief List of all entity IDs belonging to this scene. */
        std::unordered_map<EntityID, Entity> mEntities;

        std::unordered_map<EntityID, EntityID> mParents;
        std::unordered_map<EntityID, std::vector<EntityID>> mChildren;

        /* @brief ECS manager owning and managing components for this scene. */
        ecs::EntityManager mEntityMgr;

        void AttachChild( EntityID parent, EntityID child );
        void DetachChild( EntityID child );
        
        Optional<Entity> GetEntity( EntityID entity );
        
        Entity& CreateEntity( );
        void DeleteEntity( EntityID entity );

    };

} // namespace lum