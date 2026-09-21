#pragma once

#include "Rhi2/RhiCommon.hpp"
#include "volk.h"

namespace lum::rhi::vk {

	namespace detail {

		class DefaultVertexLayout {
		public:

			LUM_NODISCARD static const DefaultVertexLayout& GetStatic( ) noexcept {
				static DefaultVertexLayout val{};
				return val;
			}

			LUM_NODISCARD const std::array<VkVertexInputAttributeDescription, 3>& GetAttributeDescriptions( ) const {
				return m_AttributeDescriptions;
			}

			LUM_NODISCARD const VkVertexInputBindingDescription& GetBindingDescription( ) const {
				return m_VertexBindingDesciption;
			}

			LUM_NODISCARD const VkPipelineVertexInputStateCreateInfo& GetStateCreateInfo( ) const {
				return m_InputStateInfo;
			}

		private:

			DefaultVertexLayout( ) { create_defaults( ); }

			void create_defaults( ) noexcept {

				m_VertexBindingDesciption.binding = 0;
				m_VertexBindingDesciption.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
				m_VertexBindingDesciption.stride = sizeof( Vertex );

				auto& posAttr = m_AttributeDescriptions[ 0 ];
				posAttr.location = 0;
				posAttr.binding = 0;
				posAttr.format = VK_FORMAT_R32G32B32_SFLOAT;
				posAttr.offset = offsetof( Vertex, m_Position );

				auto& colAttr = m_AttributeDescriptions[ 1 ];
				colAttr.location = 1;
				colAttr.binding = 0;
				colAttr.format = VK_FORMAT_R32G32B32_SFLOAT;
				colAttr.offset = offsetof( Vertex, m_Color );

				auto& uvAttr = m_AttributeDescriptions[ 2 ];
				uvAttr.location = 2;
				uvAttr.binding = 0;
				uvAttr.format = VK_FORMAT_R32G32_SFLOAT;
				uvAttr.offset = offsetof( Vertex, m_Uv );

				m_InputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
				m_InputStateInfo.vertexAttributeDescriptionCount = m_AttributeDescriptions.size( );
				m_InputStateInfo.pVertexAttributeDescriptions = m_AttributeDescriptions.data( );
				m_InputStateInfo.vertexBindingDescriptionCount = 1;
				m_InputStateInfo.pVertexBindingDescriptions = &m_VertexBindingDesciption;

			}

			VkPipelineVertexInputStateCreateInfo m_InputStateInfo{};
			VkVertexInputBindingDescription m_VertexBindingDesciption{};
			std::array<VkVertexInputAttributeDescription, 3> m_AttributeDescriptions{};

		};

		class LogicalDeviceFeatures {
		public:
			
			LUM_NODISCARD static const LogicalDeviceFeatures& GetStatic( ) noexcept {
				static LogicalDeviceFeatures val{};
				return val;
			}

			LUM_NODISCARD const void* GetChainHead( ) const noexcept {
				return &m_Vulkan12Features;
			}

		private:

			LogicalDeviceFeatures( ) { create_defaults( ); }

			void create_defaults( ) {

				//m_Vulkan14Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES;
				//m_Vulkan14Features.dynamicRenderingLocalRead = VK_TRUE;
				//m_Vulkan14Features.pNext = nullptr;

				m_Vulkan13Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
				m_Vulkan13Features.dynamicRendering = VK_TRUE;
				m_Vulkan13Features.pNext = nullptr;

				m_Vulkan12Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
				m_Vulkan12Features.descriptorIndexing = VK_TRUE;
				m_Vulkan12Features.pNext = &m_Vulkan13Features;

			}

			//VkPhysicalDeviceVulkan14Features m_Vulkan14Features{};
			VkPhysicalDeviceVulkan13Features m_Vulkan13Features{};
			VkPhysicalDeviceVulkan12Features m_Vulkan12Features{};

		};


	}

} // namespace lum::rhi::vk