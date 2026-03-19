#include "engine.hpp"

int main( ) {

	lum::Engine e;

	e.Initialize( "C:/Users/szymek/Desktop/lumen_assets" );

	e.SetScene( "scene.lsc" );

	e.Run( );

	e.Finalize( );

}