//========= Copyright (C) 2026 3zymek, MIT License ============//
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
    struct FScene {

        /* @brief List of all entity IDs belonging to this scene. */
        std::vector<EntityID> mEntities;

        /* @brief ECS manager owning and managing components for this scene. */
        ecs::MEntityManager mEntityMgr;

    };

    /* @brief Aggregates all resource manager pointers required by the SceneManager
    *  to load and initialize scene assets.
    */
    struct FSceneManagerContext {

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
        void Initialize( FSceneManagerContext& ctx );

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
        FScene* GetCurrentScene( );

    private:
        
        /* @brief Cached context holding all resource manager references. */
        FSceneManagerContext mContext;

        /* @brief Pointer to the currently active scene. */
        FScene* mCurrentScene = nullptr;
        
        /* @brief Map of all loaded scenes, keyed by hashed scene path. */
        std::unordered_map<uint64, FScene> mScenes;

    };

} // namespace lum