/*******************************************************/
/* /                                                 \ */
/*|-------------------|LumEngine|---------------------|*/
/*|            Cross-Platform Game Engine             |*/
/*|===================================================|*/
/*|                    GitHub:                        |*/
/*|       https://github.com/3zymek/LumEngine         |*/
/*|                                                   |*/
/*|        Copyright (c) 2025-present 3zymek          |*/
/*|          Licensed under the MIT License           |*/
/* \                                                 / */
/*******************************************************/
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
		String m_ProjectName = "MyLumenProject";
		String m_ApplicationName = "MyLumenApp";

		WindowCreateInfo m_Window{};

		SafePtr<IRenderContext> m_RenderContext = nullptr;
		SafePtr<input::IInputProvider> m_InputProvider = nullptr;

		//bool m_Vsync = false;
		uint32 m_MaxFps = 60;

		//rhi::RenderBackend m_RenderingBackend;

	};

}