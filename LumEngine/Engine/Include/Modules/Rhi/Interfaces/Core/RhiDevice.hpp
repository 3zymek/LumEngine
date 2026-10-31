#pragma once

#include "Rhi/RhiTypes.hpp"
#include "Rhi/Interfaces/RhiAdapter.hpp"
#include "Rhi/Interfaces/Core/RhiBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"
#include "Rhi/Interfaces/Core/RhiImage.hpp"
#include "Rhi/Interfaces/Core/RhiSampler.hpp"
#include "Rhi/Interfaces/Core/RhiCommandBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiResourceSet.hpp"
#include "Core/Utils/SafePtr.hpp"

namespace lum {

	class ISurfaceProvider;

	namespace ev {
		class EventBus;
	} // namespace ev

} // namespace lum

namespace lum::rhi {

	struct RenderDeviceCreateInfo {

		std::vector<const char*>	m_InstanceExtensions{};
		void* m_NativeWindowHandle = nullptr;
		bool						m_EnableValidation = false;
		AdapterRequirements			m_AdapterRequirements{};
		SafePtr<ISurfaceProvider>	m_SurfaceProvider = nullptr; // REQUIRED
		SafePtr<ev::EventBus>		m_EventBus = nullptr; // REQUIRED


	};

	class IRenderDevice {
	public:

		virtual void Initialize( const RenderDeviceCreateInfo& info ) noexcept = 0;
		virtual void Finalize( ) noexcept = 0;

		virtual void UpdateFrame( ) noexcept = 0;

		virtual BufferHandle CreateBuffer( const BufferCreateInfo& info ) noexcept = 0;
		virtual void DestroyBuffer( const BufferHandle handle ) noexcept = 0;

		virtual PipelineHandle CreatePipeline( const PipelineCreateInfo& info ) noexcept = 0;
		virtual void DestroyPipeline( const PipelineHandle handle ) noexcept = 0;

		virtual ImageHandle CreateImage( const ImageCreateInfo& info ) noexcept = 0;
		virtual void DestroyImage( const ImageHandle handle ) noexcept = 0;

		virtual SamplerHandle CreateSampler( const SamplerCreateInfo& info ) noexcept = 0;
		virtual void DestroySampler( const SamplerHandle handle ) noexcept = 0;

		virtual ResourceLayoutHandle CreateResourceLayout( const ResourceLayoutCreateInfo& info ) noexcept = 0;
		virtual void DestroyResourceLayout( const ResourceLayoutHandle handle ) noexcept = 0;

		virtual ResourceSetHandle CreateResourceSet( const ResourceSetCreateInfo& info ) noexcept = 0;
		virtual void DestroyResourceSet( const ResourceSetHandle handle ) noexcept = 0;

		virtual void SendCommandBuffer( CommandBuffer& buffer ) noexcept = 0;

		virtual ~IRenderDevice( ) = default;

	};

} // namespace lum::rhi