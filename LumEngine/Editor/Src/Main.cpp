#include "Core/EditorCamera.hpp"
#include "Core/Utils/OwningPtr.hpp"
#include "Core/Utils/ResourceLoader.hpp"
#include "Event/EventBus.hpp"
#include "Platform/GLFWVulkanSurfaceProvider.hpp"
#include "Platform/Window.hpp"
#include "Rhi/Backends/Vulkan/Core/RhiDevice_vulkan.hpp"
#include "Rhi/Interfaces/Core/RhiCommandTypes.hpp"
#include "Rhi/Interfaces/Core/RhiDevice.hpp"
#include <Core/CoreTypes.hpp>
#include <glfw3.h>
#include <Rhi/RhiTypes.hpp>
#include <vector>
#include "Core/Math/Mat4.hpp"
#include "Rendering/VertexLayoutConfig.hpp"
#include "Engine.hpp"
#include <filesystem>

int main(int argc, char* argv[]) {

	/*
	// ResourceLoader resolves engine assets relative to the working directory.
	// Anchor it to the executable so launching from Terminal's home directory works.
	if (argc > 0 && argv[0] && std::filesystem::path(argv[0]).has_parent_path()) {
		std::error_code error{};
		auto executablePath = std::filesystem::absolute(argv[0], error);
		if (!error)
			std::filesystem::current_path(executablePath.parent_path(), error);
	}
	// Refresh roots initialized before main() after anchoring the working directory.
	lum::ResourceLoader::SetRoot(std::filesystem::current_path().parent_path() / "LumEngine");
	*/

	lum::EngineCreateInfo info{};
	info.m_ApplicationName = "LumEngine";
#if defined(WIN32)
	info.m_ProjectDir = "C:/Users/szymek/Desktop/lumen_assets";
#elif defined(__APPLE__)
	info.m_ProjectDir = "/Users/br.eeziak/Desktop/lumen_assets";
#endif

	lum::Engine engine{};

	engine.Initialize(info);
	lum::editor::EditorCamera editorCamera{};

	while (engine.Running()) {

		engine.BeginFrame();
		editorCamera.Update(engine.GetDeltaTime(), engine.Render().m_Renderer);

		engine.Tick();

		engine.EndFrame();

	}

	engine.Finalize();

}