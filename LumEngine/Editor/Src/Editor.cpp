#include "Editor.hpp"
#include <QTimer>
#include <QTextEdit>
#include "Platform/QtContextCreator.hpp"
#include "Event/Events/WindowEvents.hpp"
#include "Render/Renderer.hpp"

namespace lum::editor {

	void Editor::Initialize( ) {

		m_Window = new QMainWindow( );
		m_Window->setWindowTitle( "LumEngine Editor" );
		m_Window->resize( 1280, 720 );

		mRenderContext = QtContextCreator::Create( QtContextCreator::Api::OpenGL );
		mRenderContext->Initialize( );

		EngineCreateInfo info{};
		info.m_ProjectDir = "C:\\Users\\szymek\\Desktop\\lumen_assets";
		info.m_RenderContext = mRenderContext.get( );
		info.m_Window.m_Width = 1280;
		info.m_Window.m_Height = 720;
		mEngine.Initialize( info );

		mEngine.Scene( ).m_SceneMgr.LoadScene( "scene.lsc" );
		mEngine.Scene( ).m_SceneMgr.SetScene( "scene.lsc" );

		{
			ViewportCreateInfo	info{};
			info.mRenderDevice	= mEngine.Platform( ).m_RenderDevice.get( );
			info.mRenderContext = mRenderContext.get( );
			info.mEventBus		= mEngine.EventBus( );
			mViewport.Initialize( info );
		}

		m_Window->setCentralWidget( &mViewport );
		m_Window->show( );

		EWindowResized initialResizeEvent{};
		initialResizeEvent.m_Width = 1280;
		initialResizeEvent.m_Height = 720;
		//mEngine.EventBus( ).Emit( initialResizeEvent );

		mViewport.SetTexture( mEngine.Render( ).m_Renderer.GetFrameTexture( ) );

		QTimer* timer = new QTimer( m_Window );
		QObject::connect(
			timer,
			&QTimer::timeout,
			[ & ]( ) {
				
				mRenderContext->MakeCurrent( );

				mEngine.BeginFrame( );

				render::RenderCamera dummyCamera{};
				dummyCamera.m_Position = Vector3( 0.0f, 0.0f, 5.0f );
				dummyCamera.m_View = LookAt( dummyCamera.m_Position, Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ) );
				dummyCamera.m_Projection = Perspective( Radians( 60.0f ), 1280.0f / 720.0f, 0.1f, 1000.0f );

				mEngine.Render( ).m_Renderer.UpdateCamera( dummyCamera );

				mEngine.Tick( );
				mEngine.EndFrame( );

				mViewport.SetTexture( mEngine.Render( ).m_Renderer.GetFrameTexture( ) );
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