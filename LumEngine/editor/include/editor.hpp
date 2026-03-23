#pragma once

#include "engine.hpp"
<<<<<<< HEAD
#include "editor_camera.hpp"
=======
>>>>>>> f8ece7c (Editor in works)

namespace lum {

	class Editor {
	public:
<<<<<<< HEAD

		void Initialize( ) {

			mEngine.Initialize( "C:/Users/szymek/Desktop/lumen_assets" ); mEngine.SetScene( "scene.lsc" );

			mCamera.Initialize( mEngine.GetEventBus( ) );

			init_imgui( &mEngine.GetPlatform( ).mWindow );

		}
=======
		
		void Initialize( ) { mEngine.Initialize( "C:/Users/szymek/Desktop/lumen_assets" ); mEngine.SetScene( "scene.lsc" ); }
>>>>>>> f8ece7c (Editor in works)
		void Finalize( ) { mEngine.Finalize( ); }
		void Run( ) ;
		void DrawSceneHierarchy( );
		void DrawInspector( );
		void DrawViewport( ) ;
<<<<<<< HEAD

	private:

		Engine mEngine;
		EditorCamera mCamera;
		glm::vec2 mViewportSize = glm::vec2( 0, 0 );
		bool bLayoutInitialized = false;


		void init_imgui( Window* window );
		void begin_imgui( );
		void end_imgui( );
=======
		
	private:

		Engine mEngine;
>>>>>>> f8ece7c (Editor in works)

	};

}