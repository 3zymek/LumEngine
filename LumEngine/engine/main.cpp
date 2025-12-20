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

    render::RenderInitParams params;
    params.fullscreen = false;
    params.MSAA_samples = 4;
    params.title = "test";

    render::Renderer render;
    render.Init(params);

    while (render.WindowIsOpen()) {
        render.BeginFrame();


        render.EndFrame();
    }


    return 0;
}