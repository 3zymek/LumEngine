//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Platform module — window and render device lifecycle.
//
//=============================================================================//
#pragma once

#include "Platform/RenderContext.hpp"
#include "Platform/GLFWOpenGLContext.hpp"
#include "Platform/Window.hpp"
#include "CreateInfo.hpp"
#include "Core/Utils/SafePtr.hpp"

namespace lum {

	namespace rhi { class IRenderDevice; }

	struct PlatformModuleCreateInfo {
		SafePtr<IRenderContext> m_RenderContext = nullptr;
		SafePtr<input::IInputProvider> m_InputProvider = nullptr;
	};

	// Owns the OS window and the RHI render device.
	// Must be initialized before any rendering or resource modules.
	class LUM_API PlatformModule {
	public:

		Optional<Window> m_Window = Optional<Window>::Empty();
		std::unique_ptr<rhi::IRenderDevice> m_RenderDevice = nullptr;

		SafePtr<IRenderContext> m_RenderContext = nullptr;
		std::unique_ptr<GLFWOpenGLContext> m_DefaultContext = nullptr;

		void Initialize( EngineCreateInfo info, ev::EventBus& bus );
		void Finalize( );

	};

} // namespace lum