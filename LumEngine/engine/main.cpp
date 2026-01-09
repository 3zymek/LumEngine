#include "core/core_pch.hpp"
#include "lum_packages/lum_audio.hpp"
#include "lum_packages/lum_ecs.hpp"
#include "lum_packages/lum_events.hpp"
#include "lum_packages/lum_render.hpp"
#include "window_context/input_common.hpp"
#include "core/logger.hpp"
#include "editor.hpp"
#include "render/components/c_mesh.hpp"
#include "audio/components/c_audio_listener.hpp"
#include "window_context/window.hpp"
using namespace lum;
struct Bad {
    LumComponentTag;
    uint32_t id;
    std::string name;
};

int main() {

    Logger::Get().EnableLog(LogSeverity::DEBUG);
    
    WindowDescriptor desc;
    auto* window = CreateWindow(desc);

    while (window->IsOpen()) {

        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();

        glfwSwapBuffers(static_cast<GLFWwindow*>(window->GetNativeWindow()));

    }

    /*
    core::EngineConfiguration cfg;

    cfg.render_config.fullscreen = false;
    cfg.render_config.height = 1280;
    cfg.render_config.width = 1920;
    cfg.render_config.title = "LumEngine Editor";

    core::Engine engine{ cfg };
    editor::Editor editor{ engine };
    Entity e = engine.GetECSManager().CreateEntity();
    e.AddComponent<ecs::components::TransformComponent>();
    e.AddComponent<ecs::components::MeshComponent>();

    auto& audio = engine.GetAudioManager();
    audio.LoadSound("01", "here_with_me.mp3");
    auto emitter = audio.CreateEmitter(e);
    emitter.Add("01");
    emitter.Play("01").SetVolume("01", 0.0);
    editor.Run();
    */

}
