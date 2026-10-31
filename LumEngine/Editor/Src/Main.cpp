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
#include "Core/Math/Mat4.hpp";

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


	struct CameraData {
		Matrix4 m_Mvp = Matrix4( 1.0f );
	} cameraData;
	cameraData.m_Mvp[ 3 ][ 0 ] = 0.5f;

	BufferCreateInfo uboInfo{};
	uboInfo.m_BufferSize = sizeof( CameraData );
	uboInfo.m_Usage = BufferUsage::Uniform;
	uboInfo.m_Data = &cameraData;
	uboInfo.m_DataSize = sizeof( CameraData );

	auto ubo = device( ).CreateBuffer( uboInfo );


	rhi::ResourceSetBinding imageBinding{};
	imageBinding.m_Binding = 0;
	imageBinding.m_ResourceCount = 1;
	imageBinding.m_ShaderStages = ShaderStage::Fragment;
	imageBinding.m_Type = ResourceType::CombinedImageSampler;

	rhi::ResourceSetBinding cameraBinding{};
	cameraBinding.m_Binding = 1;
	cameraBinding.m_ResourceCount = 1;
	cameraBinding.m_ShaderStages = ShaderStage::Vertex;
	cameraBinding.m_Type = ResourceType::UniformBuffer;

	rhi::ResourceLayoutCreateInfo layoutInfo{};
	layoutInfo.m_Bindings.push_back( imageBinding );
	layoutInfo.m_Bindings.push_back( cameraBinding );

	auto layout = device( ).CreateResourceLayout( layoutInfo );

	rhi::ResourceSetCreateInfo resInfo{};
	resInfo.m_Layout = layout;

	auto resSet = device( ).CreateResourceSet( resInfo );





	rhi::PipelineCreateInfo pipInfo{};

	pipInfo.m_Layout.m_ResourceLayouts.push_back( layout );

	auto& assembly = pipInfo.m_AssemblyPass;
	assembly.m_PrimitiveRestart = false;
	assembly.m_Topology = rhi::PrimitiveTopology::TriangleList;

	auto& rast = pipInfo.m_RasterizationPass;
	rast.m_CullMode = rhi::CullMode::None;
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
	cmdBindVbo.m_BufferHandles = std::span<rhi::BufferHandle>( &vbo, 1 );

	rhi::cmd::BindElementBuffer cmdBindEbo{};
	cmdBindEbo.m_BufferHandle = ebo;

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
		ResourceRoot::External, "test.png", ImageChannels::RGBA
	);
	if (!dataRes)
		LUM_LOG_FATAL( "{}", dataRes.GetError( ) );

	auto data = dataRes.ValueRef( );


	rhi::ImageCreateInfo imageInfo{};
	imageInfo.m_Dimension = ImageDimension::Image2D;
	imageInfo.m_Extent = { data.m_Width, data.m_Height, 1 };
	imageInfo.m_Format = ImageFormat::RGBA8_SRGB;
	imageInfo.m_UsageFlags = ImageUsageFlag::ShaderReadable | ImageUsageFlag::TransferDestination;
	imageInfo.m_MipmapLevels = 1;
	imageInfo.m_ViewType = ImageViewType::Default;
	imageInfo.m_ArrayLayers = 1;

	auto image = device( ).CreateImage( imageInfo );


	rhi::SamplerCreateInfo samplerInfo{};
	auto sampler = device( ).CreateSampler( samplerInfo );
	
	cmd::UpdateBufferInfo updateBuff{};
	updateBuff.m_ArrayIndex = 0;
	updateBuff.m_Binding = 1;
	updateBuff.m_BufferHandle = ubo;
	updateBuff.m_Offset = 0;

	cmd::UpdateImageInfo updateImg{};
	updateImg.m_ArrayIndex = 0;
	updateImg.m_Binding = 0;
	updateImg.m_ImageHandle = image;
	updateImg.m_SamplerHandle = sampler;

	cmd::UpdateResourceSet cmdUpdateResSet{};
	cmdUpdateResSet.m_SetHandle = resSet;
	cmdUpdateResSet.m_BufferInfos.push_back( updateBuff );
	cmdUpdateResSet.m_ImageInfos.push_back( updateImg );
	
	cmd::BindResourceSet cmdBindResSet{};
	cmdBindResSet.m_SetHandle = resSet;


	cmd::TransferImage cmdTransferImg{};
	cmdTransferImg.m_Data = data;
	cmdTransferImg.m_Image = image;


	rhi::CommandBuffer cmdBuffer{};

	cmdBuffer.Push( cmdUpdateResSet );
	cmdBuffer.Push( cmdTransferImg );

	while (window.IsOpen( )) { 

		cmdBuffer.Push( cmdBindPip );
		cmdBuffer.Push( cmdBindVbo );
		cmdBuffer.Push( cmdBindEbo );
		cmdBuffer.Push( cmdBindResSet );
		cmdBuffer.Push( cmdSetViewport );
		cmdBuffer.Push( cmdSetScissors );
		cmdBuffer.Push( cmdDrawIndexed );

		device( ).SendCommandBuffer( cmdBuffer );
		cmdBuffer.Clear( );

		window.Update( );
		device( ).UpdateFrame( );


	}

	device( ).Finalize( );


}