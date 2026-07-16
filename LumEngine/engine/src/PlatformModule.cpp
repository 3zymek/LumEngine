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

	void MPlatformModule::Initialize( ev::EventBus& bus ) {

		WindowCreateInfo desc;
		desc.mEventBus = &bus;
		desc.mIconData = ResourceLoader::LoadImageFromFile( ResourceRoot::Internal, "branding/lumengine-icon.png", 4 );

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