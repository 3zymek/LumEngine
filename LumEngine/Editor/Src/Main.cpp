//#include "Editor.hpp"
#include "Rhi2/Interfaces/Core/RhiDevice.hpp"
#include "Rhi2/Backends/Vulkan/Core/RhiDevice_vulkan.hpp"
#include "Core/Utils/OwningPtr.hpp"
#include "Platform/VulkanSurfaceProvider.hpp"
#include "Platform/GLFWVulkanSurfaceProvider.hpp"
using namespace lum;
int main( int argc, char* argv[ ] ) {

    OwningPtr<rhi::IRenderDevice> device = OwningPtr<rhi::vk::VulkanDevice>::Create( );

    glfwInit( );   
    glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
 
    GLFWwindow* window = glfwCreateWindow( 800, 800, "test", nullptr, nullptr );
    
    OwningPtr<GLFWVulkanSurfaceProvider> surfaceProvider = OwningPtr<GLFWVulkanSurfaceProvider>::Create( window );

    uint32 count{};
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions( &count );
    std::vector<const char*> extensions( glfwExtensions, glfwExtensions + count );

    rhi::RenderDeviceCreateInfo info{};
    info.m_InstanceExtensions = extensions;
    info.m_EnableValidation = true;
    info.m_SurfaceProvider = surfaceProvider;

    device( ).Initialize( info );

    while (true) {
        device( ).DrawFrame( );
    }

    device( ).Finalize( );

    

    /*

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
    */
    
}