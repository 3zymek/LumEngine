//#include "Editor.hpp"
#include "Core/Utils/OwningPtr.hpp"
#include "Core/Utils/ResourceLoader.hpp"
#include "Event/EventBus.hpp"
#include "Platform/GLFWVulkanSurfaceProvider.hpp"
#include "Platform/Window.hpp"
#include "Rhi/Backends/Vulkan/Core/RhiDevice_vulkan.hpp"
#include "Rhi/Interfaces/Core/RhiCommandTypes.hpp"
#include "Rhi/Interfaces/Core/RhiDevice.hpp"
#include <Core/CoreTypes.hpp>
#include <glfw3.h>
#include <Rhi/RhiTypes.hpp>
#include <vector>

using namespace lum;
using namespace lum::rhi;

static std::vector<uint32> s_Indices = {
		0, 1, 2,
		2, 3, 0
};

static std::vector<rhi::Vertex> s_Vertices = {
	// Bottom-left
	{
		{ -0.5f, -0.5f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f }
	},
	// Bottom-right
	{
		{ 0.5f, -0.5f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 1.0f, 0.0f }
	},
	// Top-right
	{
		{ 0.5f, 0.5f, 0.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f }
	},
	// Top-left
	{
		{ -0.5f, 0.5f, 0.0f },
		{ 1.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f }
	}
};

int main( int argc, char* argv[ ] ) {

#	if defined(_WIN32)
	ResourceLoader::SetProjectRoot( "C:/Users/szymek/Desktop/lumen_assets" );
#	elif defined(__APPLE__)
	ResourceLoader::SetProjectRoot( "/Users/br.eeziak/Desktop/glsl" );
#	endif

	OwningPtr<rhi::IRenderDevice> device = OwningPtr<rhi::vk::VulkanDevice>::Create( );

	ev::EventBus bus{};

	Window window{};
	WindowCreateInfo winInfo{};
	winInfo.m_Title = "Vulkan Development";
	winInfo.m_Height = 800;
	winInfo.m_Width = 800;
	window.Initialize( winInfo, bus );
	OwningPtr<GLFWVulkanSurfaceProvider> surfaceProvider = OwningPtr<GLFWVulkanSurfaceProvider>::Create( reinterpret_cast<GLFWwindow*>(window.GetNativeWindow( )) );

	uint32 count{};
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions( &count );
	std::vector<const char*> extensions( glfwExtensions, glfwExtensions + count );

	rhi::RenderDeviceCreateInfo deviceInfo{};
	deviceInfo.m_InstanceExtensions = extensions;
	deviceInfo.m_EnableValidation = true;
	deviceInfo.m_SurfaceProvider = surfaceProvider;
	deviceInfo.m_EventBus = bus;

	device( ).Initialize( deviceInfo );


	rhi::PipelineCreateInfo pipInfo{};

	auto& assembly = pipInfo.m_AssemblyPass;
	assembly.m_PrimitiveRestart = false;
	assembly.m_Topology = rhi::PrimitiveTopology::TriangleList;

	auto& rast = pipInfo.m_RasterizationPass;
	rast.m_CullMode = rhi::CullMode::Back;
	rast.m_DepthBiasEnabled = false;
	rast.m_PolygonMode = rhi::PolygonMode::Fill;
	rast.m_FrontFace = rhi::FrontFace::Clockwise;
	rast.m_RasterizerEnabled = true;

	auto& blend = pipInfo.m_ColorBlendPass;
	rhi::ColorBlendAttachment attachment{};
	attachment.m_BlendEnabled = false;
	attachment.m_ColorMask = rhi::ColorComponentFlag::RGBA;
	blend.m_Attachments.push_back( attachment );

	pipInfo.m_VertexInputPass = rhi::VertexLayoutConfiguration::GetStatic( ).GetVertexInputPass( );

	rhi::ShaderInfo vertexInfo{};
	vertexInfo.m_EntryPoint = "main";
	vertexInfo.m_Path = ResourceLoader::ResolveResourcePath( ResourceRoot::External, "debug.vert.spv" );
	vertexInfo.m_Stage = rhi::ShaderStage::Vertex;
	pipInfo.m_ShaderInfos.push_back( vertexInfo );

	rhi::ShaderInfo fragmentInfo{};
	fragmentInfo.m_EntryPoint = "main";
	fragmentInfo.m_Path = ResourceLoader::ResolveResourcePath( ResourceRoot::External, "debug.frag.spv" );
	fragmentInfo.m_Stage = rhi::ShaderStage::Fragment;
	pipInfo.m_ShaderInfos.push_back( fragmentInfo );

	auto pipeline = device( ).CreatePipeline( pipInfo );



	rhi::BufferCreateInfo vboInfo{};
	vboInfo.m_DataSize = s_Vertices.size( ) * sizeof( rhi::Vertex );
	vboInfo.m_BufferSize = s_Vertices.size( ) * sizeof( rhi::Vertex );
	vboInfo.m_Data = s_Vertices.data( );
	vboInfo.m_Usage = rhi::BufferUsage::Vertex;
	auto vbo = device( ).CreateBuffer( vboInfo );

	rhi::BufferCreateInfo eboInfo{};
	eboInfo.m_DataSize = s_Indices.size( ) * sizeof( uint32 );
	eboInfo.m_BufferSize = s_Indices.size( ) * sizeof( uint32 );
	eboInfo.m_Data = s_Indices.data( );
	eboInfo.m_Usage = rhi::BufferUsage::Element;
	auto ebo = device( ).CreateBuffer( eboInfo );

	rhi::cmd::BindGraphicsPipeline cmdBindPip{};
	cmdBindPip.m_Handle = pipeline;

	rhi::cmd::BindVertexBuffers cmdBindVbo{};
	cmdBindVbo.m_Handles = std::span<rhi::BufferHandle>( &vbo, 1 );

	rhi::cmd::BindElementBuffer cmdBindEbo{};
	cmdBindEbo.m_Handle = ebo;

	rhi::cmd::DrawIndexed cmdDrawIndexed{};
	cmdDrawIndexed.m_FirstIndex = 0;
	cmdDrawIndexed.m_FirstInstance = 0;
	cmdDrawIndexed.m_IndexCount = s_Indices.size( );
	cmdDrawIndexed.m_InstanceCount = 1;
	cmdDrawIndexed.m_VertexOffset = 0;

	rhi::cmd::SetScissors cmdSetScissors{};

	rhi::cmd::SetViewport cmdSetViewport{};


	// MAIN CODE

	auto dataRes = ResourceLoader::LoadImageFromFile(
		ResourceRoot::External, "test.png"
	);
	if (!dataRes)
		LUM_LOG_FATAL( "{}", dataRes.GetError( ) );

	auto data = dataRes.ValueRef( );


	rhi::ImageCreateInfo imageInfo{};
	imageInfo.m_Dimension = ImageDimension::Image2D;
	imageInfo.m_Extent = { data.m_Width, data.m_Height, 1 };
	imageInfo.m_Format = ImageFormat::RGBA32_FLOAT;
	imageInfo.m_MipmapLevels = 1;
	imageInfo.m_ViewType = ImageViewType::Default;
	imageInfo.m_UsageFlags = ImageUsageFlag::ShaderReadable;
	imageInfo.m_ArrayLayers = 1;

	auto image = device( ).CreateImage( imageInfo );


	rhi::SamplerCreateInfo samplerInfo{};
	auto sampler = device( ).CreateSampler( samplerInfo );

	rhi::ResourceSetBinding binding{};
	binding.m_Binding = 0;
	binding.m_ResourceCount = 1;
	binding.m_ShaderStages = ShaderStage::Vertex;
	binding.m_Type = ResourceType::CombinedImageSampler;

	rhi::ResourceSetCreateInfo resInfo{};
	resInfo.m_Bindings.push_back( binding );

	auto resSet = device( ).CreateResourceSet( resInfo );
	
	cmd::UpdateResourceSet cmdUpdateResSet{};
	cmdUpdateResSet.m_ArrayIndex = 0;
	cmdUpdateResSet.m_Binding = 0;
	cmdUpdateResSet.m_SetHandle = resSet;
	cmdUpdateResSet.m_Image.m_ImageHandle = image;
	cmdUpdateResSet.m_Image.m_SamplerHandle = sampler;
	


	cmd::TransferImage cmdTransferImg{};
	cmdTransferImg.m_Data = data;
	cmdTransferImg.m_Image = image;
	cmdTransferImg.m_StagingOffset = 0;


	rhi::CommandBuffer cmdBuffer{};

	cmdBuffer.Push( cmdUpdateResSet );
	cmdBuffer.Push( cmdTransferImg );

	while (window.IsOpen( )) {

		cmdBuffer.Push( cmdBindPip );
		cmdBuffer.Push( cmdBindVbo );
		cmdBuffer.Push( cmdBindEbo );
		cmdBuffer.Push( cmdSetViewport );
		cmdBuffer.Push( cmdSetScissors );
		cmdBuffer.Push( cmdDrawIndexed );

		device( ).SendCommandBuffer( cmdBuffer );
		cmdBuffer.Clear( );

		window.Update( );
		device( ).UpdateFrame( );


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