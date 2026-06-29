#include "editor.hpp"

int main( ) {

	//lum::render::detail::LightSpaceMatricesGPU

	lum::editor::Editor editor;
	editor.Initialize( );
	editor.Run( );
	editor.Finalize( );

}