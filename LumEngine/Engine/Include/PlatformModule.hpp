//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Platform module — window and render device lifecycle.
//
//=============================================================================//
#pragma once

#include "Platform/Window.hpp"
#include "CreateInfo.hpp"
#include "Rhi/GLFWContext.hpp"
#include "Core/Utils/SafePtr.hpp"

namespace lum {

	namespace rhi { class RenderDevice; }

	// Owns the OS window and the RHI render device.
	// Must be initialized before any rendering or resource modules.
	class LUM_API PlatformModule {
	public:

		Optional<Window> mWindow = Optional<Window>::Empty();
		std::unique_ptr<rhi::RenderDevice> mRenderDevice = nullptr;

		SafePtr<RenderContext> mRenderContext = nullptr;
		std::unique_ptr<GLFWContext> mDefaultContext = nullptr;

		void Initialize( EngineCreateInfo info, ev::EventBus& bus );
		void Finalize( );

	};

} // namespace lum