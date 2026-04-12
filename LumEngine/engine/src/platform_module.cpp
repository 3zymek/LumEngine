//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Platform module — window and render device lifecycle.
//
//=============================================================================//

#include "platform_module.hpp"
#include "platform/input_common.hpp"
#include "rhi/core/rhi_device.hpp"

namespace lum {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void MPlatformModule::Initialize( ev::EventBus& bus ) {

		WindowDescriptor desc;
		desc.mEventBus = &bus;

		mWindow.Initialize( desc );
		input::SetActiveWindow( static_cast< GLFWwindow* >(mWindow.GetNativeWindow( )) );

		mRenderDevice = rhi::CreateDevice( rhi::RenderBackend::OpenGL );
		mRenderDevice->Initialize( &mWindow );

	}

	void MPlatformModule::Finalize( ) {
		mRenderDevice->Finalize( );
		delete mRenderDevice;
	}

} // namespace lum