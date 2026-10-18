#pragma once

#include "Core/CoreTypes.hpp"
#include "Core/Utils/SafePtr.hpp"
#include "Platform/FileSystem/Path.hpp"
#include "Platform/Window.hpp"

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
		
		//rhi::RenderBackend m_RenderingBackend;

	};

}