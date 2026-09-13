//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Platform module — window and render device lifecycle.
//
//=============================================================================//
#pragma once

#include "Platform/Window.hpp"
#include "CreateInfo.hpp"
#include "Core/Utils/SafePtr.hpp"
#include "Core/Utils/OwningPtr.hpp"

namespace lum {

	namespace rhi { class IRenderDevice; }

	/* @brief Configuration parameters for initializing the platform lifecycle module.
	*
	* Defines external overrides and input bindings for windowing, render context
	* management, and OS event processing.
	*/
	struct PlatformModuleCreateInfo {

		/* @brief Optional external render context override.
		*
		* If provided (non-null), PlatformModule skips creating m_DefaultRenderContext
		* and binds m_ActiveRenderContext directly to this pointer. Leave nullptr to
		* allow the engine to instantiate its default context and create all essentials for rendering.
		*/
		SafePtr<IRenderContext> m_CustomRenderContext = nullptr;

		/* @brief Optional custom input handling subsystem.
		*
		* Allows overriding the default OS window input polling provider with a
		* user-defined or mock input pipeline.
		*/
		SafePtr<input::IInputProvider> m_CustomInputProvider = nullptr;

	};

	// Owns the OS window and the RHI render device.
	// Must be initialized before any rendering or resource modules.
	class LUM_API PlatformModule {
	public:

		Optional<Window> m_Window = Optional<Window>::Empty();

		OwningPtr<rhi::IRenderDevice> m_RenderDevice = nullptr;

		void Initialize( EngineCreateInfo info, ev::EventBus& bus );
		void Finalize( );

	};

} // namespace lum