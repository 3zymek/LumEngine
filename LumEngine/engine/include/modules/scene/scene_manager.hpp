//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Scene management — stores, loads and provides access to active scenes.
//
//=============================================================================//
#pragma once
#include "core/core_pch.hpp"
#include "entity/entity_manager.hpp"

namespace lum {

    //// Forward Declare ////
    class MTextureManager;
    class MMaterialManager;
    class MMeshManager;
    class MShaderManager;
    class MAudioManager;
    namespace render { class Renderer; }
    ////////////////////////


    /* @brief Represents a single scene — holds a list of active entities
    *  and their corresponding ECS manager.
    */
    struct Scene {

        /* @brief List of all entity IDs belonging to this scene. */
        std::vector<EntityID> mEntities;

        std::unordered_map<EntityID, EntityID> mParents;
        std::unordered_map<EntityID, std::vector<EntityID>> mChildren;

        /* @brief ECS manager owning and managing components for this scene. */
        ecs::MEntityManager mEntityMgr;

        void AttachChild( EntityID parent, EntityID child ) {
            if (parent == child) return;
            if (mParents.contains( child )) return;
            EntityID current = parent;
            while (mParents.contains( current )) {
                current = mParents[ current ];
                if (current == child) return;
            }
            mParents[ child ] = parent;
            mChildren[ parent ].push_back( child );
        }
        void DetachChild( EntityID child ) {
            if (!mParents.contains( child )) return;

            EntityID parent = mParents[ child ];
            mParents.erase( child );

            auto& children = mChildren[ parent ];
            children.erase(
                std::remove( children.begin( ), children.end( ), child ), children.end( )
            );

            if (children.empty( ))
                mChildren.erase( parent );
        }

    };

    /* @brief Aggregates all resource manager pointers required by the SceneManager
    *  to load and initialize scene assets.
    */
    struct SceneManagerContext {

        /* @brief Pointer to the active texture manager. */
        MTextureManager* mTextureMgr = nullptr;

        /* @brief Pointer to the active material manager. */
        MMaterialManager* mMaterialMgr = nullptr;

        /* @brief Pointer to the active mesh manager. */
        MMeshManager* mMeshMgr = nullptr;

        /* @brief Pointer to the active shader manager. */
        MShaderManager* mShaderMgr = nullptr;

        /* @brief Pointer to the active audio manager. */
        MAudioManager* mAudioMgr = nullptr;

        ev::EventBus* mEventBus = nullptr;

        /* @brief Pointer to the active renderer. */
        render::Renderer* mRenderer = nullptr;

    };

    /* @brief Manages loading, storing and switching between scenes.
    *  Scenes are keyed by a 64-bit hash derived from their file path.
    */
    class MSceneManager {
    public:

        /* @brief Initializes the manager with all required resource managers and renderer.
         *  @param ctx Context struct containing valid pointers to all subsystem managers.
         */
        void Initialize( SceneManagerContext& ctx );

        /* @brief Loads and sets the active scene from a given file path.
         *  If the scene is already cached, switches to it directly.
         *  @param scenePath Path to the scene file to load.
         */
        void SetScene( StringView scenePath );

        /* @brief Loads scene from a given file path.
         *  If the scene is already cached, reloads it.
         *  @param scenePath Path to the scene file to load.
         */
        void LoadScene( StringView scenePath );

        /* @brief Returns a pointer to the currently active scene.
         *  @return Pointer to the active Scene, or nullptr if none is set.
         */
        Scene* GetCurrentScene( );

    private:
        
        /* @brief Cached context holding all resource manager references. */
        SceneManagerContext mContext;

        /* @brief Pointer to the currently active scene. */
        Scene* mCurrentScene = nullptr;
        
        /* @brief Map of all loaded scenes, keyed by hashed scene path. */
        std::unordered_map<uint64, Scene> mScenes;

    };

} // namespace lum