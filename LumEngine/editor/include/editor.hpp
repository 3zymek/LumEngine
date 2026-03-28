#pragma once

#include "engine.hpp"
#include "editor_camera.hpp"

namespace lum {

	class Editor {
	public:

		void Initialize( ) {

			mEngine.Initialize( "C:/Users/szymek/Desktop/lumen_assets" ); mEngine.SetScene( "scene.lsc" );

			mCamera.Initialize( mEngine.GetEventBus( ) );

			init_imgui( &mEngine.GetPlatform( ).mWindow );

		}
		void Finalize( ) { mEngine.Finalize( ); }
		void Run( ) ;
		void DrawSceneHierarchy( );
		void DrawInspector( );
		void DrawViewport( float64 delta ) ;

	private:

		Engine mEngine;
		EditorCamera mCamera;
		glm::vec2 mViewportSize = glm::vec2( 0, 0 );
		bool bLayoutInitialized = false;


		void init_imgui( Window* window );
		void begin_imgui( );
		void end_imgui( );

	};

}