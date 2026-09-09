#include "Editor.hpp"

int main( int argc, char* argv[ ] ) {

    lum::EngineCreateInfo desc{};
    desc.m_ApplicationName = "Test";
    desc.m_ProjectName = "Vulkan";
    desc.m_ProjectDir = "C:\\Users\\szymek\\Desktop\\lumen_assets";
    desc.m_RenderContext = nullptr;
    desc.m_Window.m_Width = 1280;
    desc.m_Window.m_Height = 720;
    lum::Engine engine{};
    engine.Initialize( desc );

    while (engine.Platform( ).m_Window.Value( ).IsOpen( )) {

        engine.BeginFrame( );
        engine.Tick( );
        engine.EndFrame( );

    }
 
    return 0;
    
}