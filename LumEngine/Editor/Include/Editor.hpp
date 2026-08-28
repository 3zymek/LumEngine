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

        Editor( int argc, char* argv[ ] ) : m_QtApp( argc, argv ) {}

        void Initialize( );
        void Run( );
        void Finalize( );

    private:

        QApplication m_QtApp;
        Engine m_Engine{};

        ViewportWidget m_Viewport{};

        std::unique_ptr<IRenderContext> m_RenderContext = nullptr;

        QMainWindow* m_Window = nullptr;
        QLabel* m_Label = nullptr;

    };

}