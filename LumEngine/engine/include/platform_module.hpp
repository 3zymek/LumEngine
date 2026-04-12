//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Platform module — window and render device lifecycle.
//
//=============================================================================//
#pragma once

#include "platform/window.hpp"

namespace lum {

	namespace rhi { class RenderDevice; }

	// Owns the OS window and the RHI render device.
	// Must be initialized before any rendering or resource modules.
	class MPlatformModule {
	public:

		Window mWindow;
		rhi::RenderDevice* mRenderDevice = nullptr;

		void Initialize( ev::EventBus& bus );
		void Finalize( );

	};

} // namespace lum