#pragma once

#include "Platform/Window.hpp"
#include "Rhi/RhiCommon.hpp"
#include "Core/CoreCommon.hpp"

namespace lum {

	namespace input { class IInputProvider; }
	class IRenderContext;

	struct EngineCreateInfo {

		Path m_ProjectDir{};
		String m_ProjectName = "m_YLumenProject";
		String m_ApplicationName = "m_YLumenApp";

		WindowCreateInfo m_Window{};

		SafePtr<IRenderContext> m_RenderContext = nullptr;
		SafePtr<input::IInputProvider> m_InputProvider = nullptr;
		
		rhi::RenderBackend m_RenderingBackend;

	};

}