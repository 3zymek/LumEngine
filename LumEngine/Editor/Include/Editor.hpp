#pragma once

#include "EditorPch.hpp"

#include "CreateInfo.hpp"
#include "Engine.hpp"
#include "Platform/OpenGLContext.hpp"

namespace lum {

    class QtContext : public OpenGLContext {
    public:

        QtContext( QOpenGLContext& context, QSurface& surface )
            : mContext( &context ),
            mSurface( &surface ) {
        }

        void MakeCurrent( ) override {
            mContext->makeCurrent( mSurface );
        }

        bool Initialize( ) override {
            sContext = mContext;

            const bool result = gladLoadGLLoader(
                []( const char* name ) -> void* {
                    return reinterpret_cast<void*>(
                        sContext->getProcAddress( name )
                    );
                }
            );

            sContext = nullptr;

            return result;
        }

        void SwapBuffers( ) override {
            mContext->swapBuffers( mSurface );
        }

    private:

        QOpenGLContext* mContext = nullptr;
        QSurface* mSurface = nullptr;

        inline static QOpenGLContext* sContext = nullptr;
    };

}

namespace lum::editor {

    class Editor {
    public:

        Editor( int argc, char* argv[ ] )
            : mQtApp( argc, argv ) {
        }

        void Initialize( );
        void Run( );
        void Finalize( );

    private:

        QApplication mQtApp;
        Engine mEngine{};

        QOffscreenSurface* mSurface = nullptr;
        QOpenGLContext* mContext = nullptr;
        QtContext* mRenderContext = nullptr;

        QMainWindow* mWindow = nullptr;
        QLabel* mLabel = nullptr;

    };

}