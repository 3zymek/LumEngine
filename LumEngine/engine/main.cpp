#include "core/core_pch.hpp"
#include "lum_packages/lum_audio.hpp"
#include "lum_packages/lum_ecs.hpp"
#include "lum_packages/lum_events.hpp"
#include "lum_packages/lum_render.hpp"
#include "platform/input_common.hpp"
#include "core/utils/logger.hpp"
#include "audio/components/c_audio_listener.hpp"
#include "platform/window.hpp"
#include "rhi/core/rhi_device.hpp"
#include "core/utils/asset_loader.hpp"
#include "rhi/rhi_common.hpp"
#include "core/shaders_define.h"
#include "core/math/backend/gtx/string_cast.hpp"
#include "imgui.h"
#include "core/utils/flags.hpp"
#include "core/utils/fixed_string.hpp"
#include "render/renderer.hpp"
#include "render/texture_manager.hpp"
#include "render/mesh_manager.hpp"
#include "render/shader_manager.hpp"
#include "render/material_manager.hpp"
#include "render/render_system.hpp"

#include "engine.hpp"

#include "scene/format/common.hpp"
#include "scene/format/tokenizer.hpp"
#include "scene/format/scene_parser.hpp"

using namespace lum;
using namespace lum::rhi;

int main() {
    
    Engine e;

    e.Initialize("C:/Users/Asso3/Desktop/lumen_assets");

    e.SetScene("scene.lsc");
    
    e.Run();

    e.Finalize();

} 