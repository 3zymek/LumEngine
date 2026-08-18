#include "Editor.hpp"

int main( int argc, char* argv[ ] ) {
    lum::editor::Editor editor( argc, argv );

    editor.Initialize( );
    editor.Run( );
    editor.Finalize( );

    return 0;
}