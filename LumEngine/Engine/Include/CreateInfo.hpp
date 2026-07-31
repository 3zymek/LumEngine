#pragma once

#include "Platform/Window.hpp"
#include "Rhi/RhiCommon.hpp"
#include "Core/CoreCommon.hpp"

namespace lum {

	struct EngineCreateInfo {

		StringView projectDir = "";

		WindowCreateInfo mWindow;
		
		
		rhi::RenderBackend mRenderingBackend;
		

	};

}