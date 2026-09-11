#pragma once

#include "Rhi2/Interfaces/Core/RhiDevice.hpp"
#include "Rhi2/Backends/Vulkan/RhiCommon_vulkan.hpp"
#include "Rhi2/Backends/Vulkan/Utils/VulkanAdapterEvaluator.hpp"

namespace lum::rhi::vk {

	class VulkanDevice : public IRenderDevice {
	public:

		void Initialize( const RenderDeviceCreateInfo& info ) noexcept override;
		void Finalize( ) noexcept override;

	private:

		void create_vk_instance( const RenderDeviceCreateInfo& info ) noexcept;
		void choose_adapter( ) noexcept;

		VkInstance m_Instance{};
		
		VulkanAdapterEvaluator m_AdapterEvaluator{};
		VulkanAdapter m_Adapter{};

	};

} // namespace lum::rhi::vk