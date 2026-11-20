/*******************************************************/
/* /                                                 \ */
/*|-------------------|LumEngine|---------------------|*/
/*|            Cross-Platform Game Engine             |*/
/*|===================================================|*/
/*|                    GitHub:                        |*/
/*|       https://github.com/3zymek/LumEngine         |*/
/*|                                                   |*/
/*|        Copyright (c) 2025-present 3zymek          |*/
/*|          Licensed under the MIT License           |*/
/* \                                                 / */
/*******************************************************/
#pragma once

#include <unordered_map>

#include "Scene/Scene.hpp"
#include "Format/FormatTypes.hpp"
#include "Scene/Format/SceneDepManager.hpp"
#include "Core/Utils/SafePtr.hpp"

namespace lum {

    /* @brief Manages loading, storing and switching between scenes.
    *  Scenes are keyed by a 64-bit hash derived from their file path.
    */
    class LUM_API SceneManager {
    public:

        /* @brief Initializes the manager with all required resource managers and renderer.
        *  @param ctx Context struct containing valid pointers to all subsystem managers.
        */
        void Initialize(SceneManagerContext& ctx);

        /* @brief Loads and sets the active scene from a given file path.
        *  If the scene is already cached, switches to it directly.
        *  @param scenePath Path to the scene file to load.
        */
        void SetScene(const Path& scenePath);

        /* @brief Loads scene from a given file path.
        *  If the scene is already cached, reloads it.
        *  @param scenePath Path to the scene file to load.
        */
        void LoadScene(const Path& scenePath);

        void SaveScene(Scene& scene);

        /* @brief Returns a pointer to the currently active scene.
        *  @return Pointer to the active Scene, or nullptr if none is set.
        */
        SafePtr<Scene> GetCurrentScene();

    private:

        /* @brief Cached context holding all resource manager references. */
        SceneManagerContext m_Ctx{};

        fmt::SceneDependencyManager m_SceneDependencyMgr{};

        /* @brief Pointer to the currently active scene. */
        SafePtr<Scene> m_CurrentScene = nullptr;

        /* @brief Map of all loaded scenes, keyed by hashed scene path. */
        std::unordered_map<uint64, Scene> m_Scenes{};

    };

} // namespace lum