//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Manages loading, storing and switching between scenes.
//
//=============================================================================//
#pragma once
#include "Scene/Scene.hpp"
#include "Format/FormatCommon.hpp"

namespace lum {

    /* @brief Manages loading, storing and switching between scenes.
    *  Scenes are keyed by a 64-bit hash derived from their file path.
    */
    class LUM_API SceneManager {
    public:

        /* @brief Initializes the manager with all required resource managers and renderer.
         *  @param ctx Context struct containing valid pointers to all subsystem managers.
         */
        void Initialize( SceneManagerContext& ctx );

        /* @brief Loads and sets the active scene from a given file path.
         *  If the scene is already cached, switches to it directly.
         *  @param scenePath Path to the scene file to load.
         */
        void SetScene( const Path& scenePath );

        /* @brief Loads scene from a given file path.
         *  If the scene is already cached, reloads it.
         *  @param scenePath Path to the scene file to load.
         */
        void LoadScene( const Path& scenePath );

        void SaveScene( const Path& scenePath );

        /* @brief Returns a pointer to the currently active scene.
         *  @return Pointer to the active Scene, or nullptr if none is set.
         */
        Scene* GetCurrentScene( );

    private:
        
        /* @brief Cached context holding all resource manager references. */
        SceneManagerContext mCtx;

        /* @brief Pointer to the currently active scene. */
        Scene* mCurrentScene = nullptr;
        
        /* @brief Map of all loaded scenes, keyed by hashed scene path. */
        std::unordered_map<uint64, Scene> mScenes;

    };

} // namespace lum