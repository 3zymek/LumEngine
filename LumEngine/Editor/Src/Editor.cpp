#include "Editor.hpp"
#include <QTimer>
#include <QTextEdit>

namespace lum::editor {

	void Editor::Initialize( ) {

		mWindow = new QMainWindow( );
		mWindow->setWindowTitle( "LumEngine Editor" );
		mWindow->resize( 1280, 720 );

		QSurfaceFormat format;
		format.setRenderableType( QSurfaceFormat::OpenGL );
		format.setVersion( 4, 5 );
		format.setProfile( QSurfaceFormat::CoreProfile );

		mSurface = new QOffscreenSurface( );
		mSurface->setFormat( format );
		mSurface->create( );

		if (!mSurface->isValid( )) {
			qFatal( "Failed to create QOffscreenSurface" );
		}

		mContext = new QOpenGLContext( );
		mContext->setFormat( format );

		if (!mContext->create( )) {
			qFatal( "Failed to create QOpenGLContext" );
		}

		if (!mContext->makeCurrent( mSurface )) {
			qFatal( "Failed to make QOpenGLContext current" );
		}

		mRenderContext = new QtContext( *mContext, *mSurface );

		EngineCreateInfo info{};
		info.mRenderContext = mRenderContext;
		mEngine.Initialize( info );

		mWindow->show( );

		char buff[ 100 ]{};
		FormatString(
			buff,
			"TestFormat {}: {}",
			1, "test"
		);

		static auto* label = new QLabel( QString( buff ), mWindow );
		mWindow->setCentralWidget( label );

		QTimer* timer = new QTimer( mWindow );
		QObject::connect(
			timer,
			&QTimer::timeout,
			[ & ]( ) {
				mEngine.BeginFrame( );
				mEngine.Tick( );
				mEngine.EndFrame( );
			}
		);
		timer->start( 16 );
	}


	void Editor::Run( ) {
		mQtApp.exec( );
	}


	void Editor::Finalize( ) {

		mEngine.Finalize( );

		delete mRenderContext;
		delete mContext;
		delete mSurface;

		mRenderContext = nullptr;
		mContext = nullptr;
		mSurface = nullptr;
	}

}