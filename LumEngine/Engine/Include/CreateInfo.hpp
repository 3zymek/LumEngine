#pragma once

#include "Platform/Window.hpp"
#include "Rhi/RhiCommon.hpp"
#include "Core/CoreCommon.hpp"

namespace lum {

	namespace input { class IInputProvider; }
	class IRenderContext;

	struct EngineCreateInfo {

		Path mProjectDir{};
		String mProjectName = "MyLumenProject";
		String mApplicationName = "MyLumenApp";

		WindowCreateInfo mWindow{};

		SafePtr<IRenderContext> mRenderContext = nullptr;
		SafePtr<input::IInputProvider> mInputProvider = nullptr;
		
		rhi::RenderBackend mRenderingBackend;

	};

}