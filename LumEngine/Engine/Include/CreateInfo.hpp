#pragma once

#include "Platform/Window.hpp"
#include "Rhi/RhiCommon.hpp"
#include "Core/CoreCommon.hpp"

namespace lum {

	struct EngineCreateInfo {

		StringView mProjectDir = "";

		WindowCreateInfo mWindow;
		
		
		rhi::RenderBackend mRenderingBackend;
		

	};

}