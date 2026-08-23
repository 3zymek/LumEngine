#include "Editor.hpp"
#include <QTimer>
#include <QTextEdit>
#include "Platform/QtContextCreator.hpp"

namespace lum::editor {

	void Editor::Initialize( ) {

		mWindow = new QMainWindow( );
		mWindow->setWindowTitle( "LumEngine Editor" );
		mWindow->resize( 1280, 720 );

		mRenderContext = QtContextCreator::Create( QtContextCreator::Api::OpenGL );
		mRenderContext->Initialize( );

		EngineCreateInfo info{};
		info.mRenderContext = mRenderContext.get();
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

	}

}