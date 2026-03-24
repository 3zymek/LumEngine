#pragma once

#include "engine.hpp"

namespace lum {

	class Editor {
	public:
		
		void Initialize( ) { 
			mEngine.Initialize( "C:/Users/Asso3/Desktop/lumen_assets" );
			mEngine.SetScene( "scene.lsc" ); 
		}
		void Finalize( ) { mEngine.Finalize( ); }
		void Run( ) ;
		void DrawSceneHierarchy( );
		void DrawInspector( );
		void DrawViewport( ) ;
		
	private:

		Engine mEngine;

	};

}