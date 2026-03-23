#include "editor.hpp"

namespace lum {

	void Editor::Run( ) {

		while (mEngine.IsRunning( )) {

			mEngine.BeginFrame( );

			mEngine.Tick( );

			mEngine.EndFrame( );

		}

	}
	void Editor::DrawSceneHierarchy( ) {

	}
	void Editor::DrawInspector( ) {

	}
	void Editor::DrawViewport( ) {

	}

}