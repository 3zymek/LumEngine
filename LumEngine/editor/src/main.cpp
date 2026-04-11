#include "editor.hpp"

int main( ) {

	lum::editor::Editor editor;
	editor.Initialize( );
	editor.Run( );
	editor.Finalize( );

}