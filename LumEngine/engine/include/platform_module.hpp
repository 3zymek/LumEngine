//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Platform module — window and render device lifecycle.
//
//=============================================================================//

#pragma once

#include "platform/window.hpp"
#include "platform/input_common.hpp"

#include "rhi/core/rhi_device.hpp"

namespace lum {

	// Owns the OS window and the RHI render device.
	// Must be initialized before any rendering or resource modules.
	struct MPlatformModule {

		Window mWindow;
		rhi::RenderDevice* mRenderDevice = nullptr;

		void Initialize( ev::EventBus& bus ) {

			WindowDescriptor desc;
			desc.mEventBus = &bus;

			mWindow.Initialize( desc );
			input::SetActiveWindow( static_cast< GLFWwindow* >(mWindow.GetNativeWindow( )) );

			mRenderDevice = rhi::CreateDevice( rhi::RenderBackend::OpenGL );
			mRenderDevice->Initialize( &mWindow );

		}

		void Finalize( ) {
			mRenderDevice->Finalize( );
			delete mRenderDevice;
		}

	};

} // namespace lum