//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Platform module — window and render device lifecycle.
//
//=============================================================================//

#include "PlatformModule.hpp"
#include "Platform/Input/Input.hpp"
#include "Rhi/Core/RhiDevice.hpp"

namespace lum {

    //---------------------------------------------------------
    // Public
    //---------------------------------------------------------

    void PlatformModule::Initialize( EngineCreateInfo info, ev::EventBus& bus ) {

        info.mWindow.mTitle = info.mApplicationName;

        if (!info.mRenderContext) {

            mWindow = Window{};
            mWindow.Value().Initialize( info.mWindow, bus );

            auto* glfwWindow = static_cast<GLFWwindow*>(mWindow.Value( ).GetNativeWindow( ));

            //input::SetActiveWindow( glfwWindow );

            mDefaultContext = std::make_unique<GLFWOpenGLContext>( glfwWindow );
            mRenderContext = mDefaultContext.get();
        }
        else {
            mRenderContext = info.mRenderContext;
        }

        mRenderDevice = rhi::CreateDevice( info.mRenderingBackend );
        mRenderDevice->Initialize( mRenderContext() );

    }

    void PlatformModule::Finalize( ) {
        mRenderDevice->Finalize( );
    }

} // namespace lum