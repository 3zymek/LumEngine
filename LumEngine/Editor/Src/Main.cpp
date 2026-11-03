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
	// Pierwszy trójkąt (bliżej / z = 0.0)
	0, 1, 2,
	2, 3, 0,

	// Drugi trójkąt testowy z tyłu (głębiej / z = 0.5 lub przesunięty)
	4, 5, 6,
	6, 7, 4
};

static std::vector<rhi::Vertex> s_Vertices = {
	// --- KEBAB / PIERWSZY KWADRAT (Z = 0.0 - bliżej) ---
	// Bottom-left
	{ { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
	// Bottom-right
	{ {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
	// Top-right
	{ {  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
	// Top-left
	{ { -0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },

	// --- DRUGI KWADRAT TESTOWY Z TYŁU (Z = 0.5 - dalej, częściowo zasłonięty) ---
	// Przesunięty w prawo-dół i w głąb (Z = 0.5)
	{ { -0.2f, -0.2f, 0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
	{ {  0.8f, -0.2f, 0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
	{ {  0.8f,  0.8f, 0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
	{ { -0.2f,  0.8f, 0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }
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


	rhi::ResourceSetBinding imageBinding{};
	imageBinding.m_Binding = 0;
	imageBinding.m_ResourceCount = 1;
	imageBinding.m_ShaderStages = ShaderStage::Fragment;
	imageBinding.m_Type = ResourceType::CombinedImageSampler;

	rhi::ResourceLayoutCreateInfo layoutInfo{};
	layoutInfo.m_Bindings.push_back( imageBinding );

	auto layout = device( ).CreateResourceLayout( layoutInfo );

	rhi::ResourceSetCreateInfo resInfo{};
	resInfo.m_Layout = layout;

	auto grassSet = device( ).CreateResourceSet( resInfo );
	auto stoneSet = device( ).CreateResourceSet( resInfo );






	rhi::PipelineCreateInfo pipInfo{};

	PushConstantRange range{};
	range.m_Size = sizeof( CameraData );
	range.m_ShaderStageFlags = ShaderStage::Vertex;
	range.m_Offset = 0;

	pipInfo.m_Layout.m_ResourceLayouts.push_back( layout );
	pipInfo.m_Layout.m_PushConstantRanges.push_back( range  );

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

	rhi::cmd::SetScissors cmdSetScissors{};

	rhi::cmd::SetViewport cmdSetViewport{};


	// MAIN CODE

	auto grassTexture = ResourceLoader::LoadImageFromFile(
		ResourceRoot::External, "test.png", ImageChannels::RGBA
	);
	
	LUM_ASSERT( grassTexture, grassTexture.GetError( ) );

	auto stoneTexture = ResourceLoader::LoadImageFromFile(
		ResourceRoot::External, "stone.png", ImageChannels::RGBA
	);

	LUM_ASSERT( stoneTexture, stoneTexture.GetError( ) );

	auto& grassData = grassTexture.ValueRef( );
	auto& stoneData = stoneTexture.ValueRef( );




	rhi::ImageCreateInfo imageInfo{};
	imageInfo.m_Dimension = ImageDimension::Image2D;
	imageInfo.m_Extent = { grassData.m_Width, grassData.m_Height, 1 };
	imageInfo.m_Format = ImageFormat::RGBA8_SRGB;
	imageInfo.m_UsageFlags = ImageUsageFlag::ShaderReadable | ImageUsageFlag::TransferDestination;
	imageInfo.m_MipmapLevels = 1;
	imageInfo.m_ViewType = ImageViewType::Default;
	imageInfo.m_ArrayLayers = 1;

	auto grassImage = device( ).CreateImage( imageInfo );

	imageInfo.m_Extent = { stoneData.m_Width, stoneData.m_Height, 1 };

	auto stoneImage = device( ).CreateImage( imageInfo );


	rhi::SamplerCreateInfo samplerInfo{};
	auto sampler = device( ).CreateSampler( samplerInfo );


	cmd::UpdateImageInfo updateGrassImg{};
	updateGrassImg.m_ArrayIndex = 0;
	updateGrassImg.m_Binding = 0;
	updateGrassImg.m_ImageHandle = grassImage;
	updateGrassImg.m_SamplerHandle = sampler;

	cmd::UpdateImageInfo updateStoneImg{};
	updateStoneImg.m_ArrayIndex = 0;
	updateStoneImg.m_Binding = 0;
	updateStoneImg.m_ImageHandle = stoneImage;
	updateStoneImg.m_SamplerHandle = sampler;

	cmd::UpdateResourceSet cmdUpdateGrassSet{};
	cmdUpdateGrassSet.m_SetHandle = grassSet;
	cmdUpdateGrassSet.m_ImageInfos.push_back( updateGrassImg );

	cmd::UpdateResourceSet cmdUpdateStoneSet{};
	cmdUpdateStoneSet.m_SetHandle = stoneSet;
	cmdUpdateStoneSet.m_ImageInfos.push_back( updateStoneImg );


	cmd::TransferImage cmdTransferGrassImg{};
	cmdTransferGrassImg.m_Data = grassData;
	cmdTransferGrassImg.m_Image = grassImage;

	cmd::TransferImage cmdTransferStoneImg{};
	cmdTransferStoneImg.m_Data = stoneData;
	cmdTransferStoneImg.m_Image = stoneImage;

	cmd::PushConstants cmdPushConstants{};
	cmdPushConstants.m_Data = &cameraData;
	cmdPushConstants.m_ShaderStageFlags = ShaderStage::Vertex;
	cmdPushConstants.m_Size = sizeof( cameraData );
	cmdPushConstants.m_Offset = 0;

	rhi::CommandBuffer cmdBuffer{};

	cmdBuffer.Push( cmdUpdateGrassSet );
	cmdBuffer.Push( cmdUpdateStoneSet );
	cmdBuffer.Push( cmdTransferGrassImg );
	cmdBuffer.Push( cmdTransferStoneImg );

	while (window.IsOpen( )) {

		cmdBuffer.Push( cmdBindPip );
		cmdBuffer.Push( cmdPushConstants );
		cmdBuffer.Push( cmdBindVbo );
		cmdBuffer.Push( cmdBindEbo );
		cmdBuffer.Push( cmdSetViewport );
		cmdBuffer.Push( cmdSetScissors );
		{
			rhi::cmd::BindResourceSet bindGrassSet{ .m_SetHandle = grassSet };
			rhi::cmd::DrawIndexed drawGrass{
				.m_IndexCount = 6,
				.m_InstanceCount = 1,
				.m_FirstIndex = 0,
				.m_VertexOffset = 0
			};

			cmdBuffer.Push( bindGrassSet );
			cmdBuffer.Push( drawGrass );
		}

		{
			rhi::cmd::BindResourceSet bindStoneSet{ .m_SetHandle = stoneSet };
			rhi::cmd::DrawIndexed drawStone{
				.m_IndexCount = 6,
				.m_InstanceCount = 1,
				.m_FirstIndex = 6,
				.m_VertexOffset = 0,
			};

			cmdBuffer.Push( bindStoneSet );
			cmdBuffer.Push( drawStone );
		}

		device( ).SendCommandBuffer( cmdBuffer );
		cmdBuffer.Clear( );

		window.Update( );
		device( ).UpdateFrame( );

	}

	device( ).Finalize( );


}