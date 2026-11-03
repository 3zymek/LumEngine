/*******************************************************/
/* /                                                 \ */
/*|-------------------|LumEngine|---------------------|*/
/*|            Cross-Platform Game Engine             |*/
/*|===================================================|*/
/*|                    GitHub:                        |*/
/*|       https://github.com/3zymek/LumEngine         |*/
/*|                                                   |*/
/*|        Copyright (c) 2025-present 3zymek          |*/
/*|          Licensed under the MIT License           |*/
/* \                                                 / */
/*******************************************************/
#pragma once

#include <Vulkan/vulkan_core.h>

#include "Core/CoreTypes.hpp"
#include "Rhi/Backends/Vulkan/Core/RhiBuffer_vulkan.hpp"
#include "Rhi/Interfaces/Core/RhiBuffer.hpp"

namespace lum::rhi::vk {

	class VulkanDevice;

	class StagingBuffer {
	public:

		~StagingBuffer( );

		void Initialize( VulkanDevice& device, usize maxSize ) noexcept;
		const VulkanBuffer& GetBuffer( ) const noexcept { return m_Buffer; }

	private:

		usize m_MaxSize{};
		BufferHandle m_Handle{};
		VulkanBuffer m_Buffer{};

	};

} // namespace lum::rhi::vk