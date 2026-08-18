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

        info.mWindow.mTitle = info.mApplicationName;

        if (info.mRenderContext == nullptr) {

            mWindow.Initialize( info.mWindow, bus );

            input::SetActiveWindow(
                static_cast<GLFWwindow*>(
                    mWindow.GetNativeWindow( )
                    )
            );

            mDefaultContext = new GLFWContext(
                *static_cast<GLFWwindow*>(
                    mWindow.GetNativeWindow( )
                    )
            );

            mRenderContext = mDefaultContext;
        }
        else {
            mRenderContext = info.mRenderContext;
        }

        mRenderDevice = rhi::CreateDevice( info.mRenderingBackend );

        mRenderDevice->Initialize( mRenderContext );
    }

    void PlatformModule::Finalize( ) {
        mRenderDevice->Finalize( );
        delete mRenderDevice;

        delete mDefaultContext;
        mDefaultContext = nullptr;
    }

} // namespace lum