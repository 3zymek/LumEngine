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

		m_RenderContext = QtContextCreator::Create( QtContextCreator::Api::OpenGL );
		m_RenderContext->Initialize( );

		EngineCreateInfo info{};
		info.m_ProjectDir = "C:\\Users\\szymek\\Desktop\\lumen_assets";
		info.m_RenderContext = m_RenderContext.get( );
		info.m_Window.m_Width = 1280;
		info.m_Window.m_Height = 720;
		m_Engine.Initialize( info );

		m_Engine.Scene( ).m_Scenem_Gr.LoadScene( "scene.lsc" );
		m_Engine.Scene( ).m_Scenem_Gr.SetScene( "scene.lsc" );

		{
			ViewportCreateInfo	info{};
			info.m_RenderDevice	= m_Engine.Platform( ).m_RenderDevice.get( );
			info.m_RenderContext = m_RenderContext.get( );
			info.m_EventBus		= m_Engine.EventBus( );
			info.m_Engine		= m_Engine;
			m_Viewport.Initialize( info );
		}

		m_Window->setCentralWidget( &m_Viewport );
		m_Window->show( );

		EWindowResized initialResizeEvent{};
		initialResizeEvent.m_Width = 1280;
		initialResizeEvent.m_Height = 720;
		//mEngine.EventBus( ).Emit( initialResizeEvent );

		m_Viewport.SetTexture( m_Engine.Render( ).m_Renderer.GetFrameTexture( ) );

		QTimer* timer = new QTimer( m_Window );
		QObject::connect(
			timer,
			&QTimer::timeout,
			[ & ]( ) {
				
				m_RenderContext->MakeCurrent( );

				m_Engine.BeginFrame( );

				/*
				render::RenderCamera dummyCamera{};
				dummyCamera.m_Position = Vector3( 0.0f, 0.0f, 5.0f );
				dummyCamera.m_View = LookAt( dummyCamera.m_Position, Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ) );
				dummyCamera.m_Projection = Perspective( Radians( 60.0f ), 1280.0f / 720.0f, 0.1f, 1000.0f );

				m_Engine.Render( ).m_Renderer.UpdateCamera( dummyCamera );
				*/

				m_Engine.Tick( );
				m_Engine.EndFrame( );

				m_Viewport.SetTexture( m_Engine.Render( ).m_Renderer.GetFrameTexture( ) );
				m_Viewport.update( );

			}
		);
		timer->start( 16 );

	}

	void Editor::Run( ) {
		m_QtApp.exec( );
	}

	void Editor::Finalize( ) {

		m_Engine.Finalize( );

	}

}