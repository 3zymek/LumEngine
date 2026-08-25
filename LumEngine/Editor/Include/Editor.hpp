#pragma once

#include "EditorPch.hpp"

#include "CreateInfo.hpp"
#include "Engine.hpp"
#include "Platform/OpenGLContext.hpp"

#include "Core/Utils/Logger.hpp"
#include "Core/Utils/LumAssert.hpp"

#include "Core/ViewportWidget.hpp"

namespace lum::editor {

    class Editor {
    public:

        Editor( int argc, char* argv[ ] ) : mQtApp( argc, argv ) {}

        void Initialize( );
        void Run( );
        void Finalize( );

    private:

        QApplication mQtApp;
        Engine mEngine{};

        ViewportWidget mViewport{};

        std::unique_ptr<IRenderContext> mRenderContext = nullptr;

        QMainWindow* mWindow = nullptr;
        QLabel* mLabel = nullptr;

    };

}