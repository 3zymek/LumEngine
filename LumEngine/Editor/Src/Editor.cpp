#include "Editor.hpp"
#include <QTimer>
#include <QTextEdit>
#include "Platform/QtContextCreator.hpp"
#include "Event/Events/WindowEvents.hpp"
#include "Render/Renderer.hpp"

namespace lum::editor {

	void Editor::Initialize( ) {

		mWindow = new QMainWindow( );
		mWindow->setWindowTitle( "LumEngine Editor" );
		mWindow->resize( 1280, 720 );

		mRenderContext = QtContextCreator::Create( QtContextCreator::Api::OpenGL );
		mRenderContext->Initialize( );

		EngineCreateInfo info{};
		info.mProjectDir = "C:\\Users\\szymek\\Desktop\\lumen_assets";
		info.mRenderContext = mRenderContext.get( );
		info.mWindow.mWidth = 1280;
		info.mWindow.mHeight = 720;
		mEngine.Initialize( info );

		mEngine.Scene( ).mSceneMgr.LoadScene( "scene.lsc" );
		mEngine.Scene( ).mSceneMgr.SetScene( "scene.lsc" );

		{
			ViewportCreateInfo	info{};
			info.mRenderDevice	= mEngine.Platform( ).mRenderDevice.get( );
			info.mRenderContext = mRenderContext.get( );
			info.mEventBus		= mEngine.EventBus( );
			mViewport.Initialize( info );
		}

		mWindow->setCentralWidget( &mViewport );
		mWindow->show( );

		EWindowResized initialResizeEvent{};
		initialResizeEvent.mWidth = 1280;
		initialResizeEvent.mHeight = 720;
		//mEngine.EventBus( ).Emit( initialResizeEvent );

		mViewport.SetTexture( mEngine.Render( ).mRenderer.GetFrameTexture( ) );

		QTimer* timer = new QTimer( mWindow );
		QObject::connect(
			timer,
			&QTimer::timeout,
			[ & ]( ) {

				mRenderContext->MakeCurrent( );

				mEngine.BeginFrame( );

				render::RenderCamera dummyCamera{};
				dummyCamera.mPosition = Vector3( 0.0f, 0.0f, 5.0f );
				dummyCamera.mView = LookAt( dummyCamera.mPosition, Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ) );
				dummyCamera.mProjection = Perspective( Radians( 60.0f ), 1280.0f / 720.0f, 0.1f, 1000.0f );

				mEngine.Render( ).mRenderer.UpdateCamera( dummyCamera );

				mEngine.Tick( );
				mEngine.EndFrame( );

				mViewport.SetTexture( mEngine.Render( ).mRenderer.GetFrameTexture( ) );
				mViewport.update( );

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