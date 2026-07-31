//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Platform module — window and render device lifecycle.
//
//=============================================================================//

#include "PlatformModule.hpp"
#include "Platform/InputCommon.hpp"
#include "Rhi/Core/RhiDevice.hpp"

namespace lum {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void PlatformModule::Initialize( EngineCreateInfo info, ev::EventBus& bus ) {

		mWindow.Initialize( info.mWindow, bus );
		input::SetActiveWindow( static_cast< GLFWwindow* >(mWindow.GetNativeWindow( )) );

		mRenderDevice = rhi::CreateDevice( info.mRenderingBackend );
		mRenderDevice->Initialize( &mWindow );

	}

	void PlatformModule::Finalize( ) {
		mRenderDevice->Finalize( );
		delete mRenderDevice;
	}

} // namespace lum