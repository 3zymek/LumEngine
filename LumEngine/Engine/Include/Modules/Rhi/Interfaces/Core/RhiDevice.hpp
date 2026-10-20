#pragma once

#include "Rhi/RhiTypes.hpp"
#include "Rhi/Interfaces/RhiAdapter.hpp"
#include "Rhi/Interfaces/Core/RhiBuffer.hpp"
#include "Rhi/Interfaces/Core/RhiPipeline.hpp"
#include "Rhi/Interfaces/Core/RhiImage.hpp"
#include "Rhi/Interfaces/Core/RhiSampler.hpp"
#include "Rhi/Interfaces/Core/RhiCommandBuffer.hpp"
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

		virtual void Initialize(const RenderDeviceCreateInfo& info) noexcept = 0;
		virtual void Finalize() noexcept = 0;

		virtual void UpdateFrame() noexcept = 0;

		virtual BufferHandle CreateBuffer(const BufferCreateInfo2& info) noexcept = 0;
		virtual void DestroyBuffer(BufferHandle& handle) noexcept = 0;

		virtual PipelineHandle CreatePipeline(const PipelineCreateInfo& info) noexcept = 0;
		virtual void DestroyPipeline(PipelineHandle& handle) noexcept = 0;

		virtual ImageHandle CreateImage(const ImageCreateInfo& info) noexcept = 0;
		virtual void DestroyImage(ImageHandle& handle) noexcept = 0;

		virtual SamplerHandle CreateSampler(const SamplerCreateInfo& info) noexcept = 0;
		virtual void DestroySampler(SamplerHandle& handle) noexcept = 0;

		virtual void SendCommandBuffer(CommandBuffer&& buffer) noexcept = 0;

		virtual ~IRenderDevice() = default;

	};

} // namespace lum::rhi