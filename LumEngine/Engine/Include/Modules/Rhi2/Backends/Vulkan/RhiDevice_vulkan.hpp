#pragma once

#include "Rhi2/Interfaces/RhiDevice.hpp"

namespace lum::rhi::vk {

	class VulkanDevice : public IRenderDevice {
	public:

		void Initialize( ) noexcept override;
		void Finalize( ) noexcept override;

	};

} // namespace lum::rhi::vk