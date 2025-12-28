<<<<<<< HEAD
#include <iostream>
#include "core/utils/path_service.hpp"
#include "lum_audio.hpp"
#include "lum_ecs.hpp"
#include "lum_events.hpp"
#include "glad/glad.h"
#include "glfw3.h"
#include "rendermodule/core/renderer.hpp"
#include "rendermodule/core/mesh_manager.hpp"
#include "rendermodule/details/render_define.hpp"
int main() {

    cstd::PathService::SetRoot("assets");

=======
#include "core/core_pch.hpp"
#include "core/utils/path_service.hpp"
#include "packages/lum_audio.hpp"
#include "packages/lum_ecs.hpp"
#include "packages/lum_events.hpp"
#include "core/logger.hpp"

int main() {
    
    lum::Logger::Get().EnableLog(lum::LogSeverity::INFO);

    cstd::PathService::SetRoot("assets");

    ecs::EntityManager ecs;
    lum::AudioManager am(ecs);
    am.Init(512, FMOD_2D);
    lum::AudioSystem sys(am);
    Entity e = ecs.CreateEntity();
    e.AddComponent<AudioEmitterComponent>();
    auto emitter = am.CreateEmitter(e);
    am.LoadSound("atomic_land", "test.wav");
    emitter.Add("atomic_land");
    emitter.Play("atomic_land").SetVolume("atomic_land", 0.7f);

    while (true) {
        sys.Update();
    }


    /*
>>>>>>> dd5bcd8 (reworking events)
    render::RenderInitParams params;
    params.fullscreen = false;
    params.MSAA_samples = 4;
    params.title = "test";

    render::Renderer render;
    render.Init(params);

    render::MeshManager mm;
    std::vector<render::Vertex> vertices = { {{1, 1, 1}, {1,1,1}, {1,1,1}} };
    std::vector<render::Index> indices = { 0, 1, 2, 3 };
    render::MeshHandle handle = mm.CreateStaticMesh(vertices, indices);

    while (render.WindowIsOpen()) {
        render.BeginFrame();

        mm.DrawMesh<render::StaticMesh>(handle);

        render.EndFrame();
    }


    return 0;
<<<<<<< HEAD
}
=======
    */
}
>>>>>>> dd5bcd8 (reworking events)
