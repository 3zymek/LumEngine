#pragma once

#include "Platform/RenderContext.hpp"
#include "Platform/Window.hpp"
#include "Rhi/RhiCommon.hpp"
#include "Core/CoreCommon.hpp"

namespace lum {

	struct EngineCreateInfo {

		Path mProjectDir{};
		String mProjectName = "MyLumenProject";
		String mApplicationName = "MyLumenApp";

		WindowCreateInfo mWindow{};

		RenderContext* mRenderContext = nullptr;
		
		rhi::RenderBackend mRenderingBackend;
		

	};

}