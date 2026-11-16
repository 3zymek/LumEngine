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
#include "Core/CorePch.hpp"

// Header file impl
#include "Rendering/DeferredBuffer.hpp"

#include "Rhi/Interfaces/Core/RhiImage.hpp"
#include "Rhi/Interfaces/Core/RhiDevice.hpp"
#include "Core/CoreTypes.hpp"

namespace lum {

	void DeferredBuffer::RecreatePrimitives( SafePtr<rhi::IRenderDevice> renderDevice, rhi::Extent2D extent ) {
		create_assets( renderDevice, extent );

	}

	rhi::ImageHandle DeferredBuffer::GetAttachment( DeferredAttachment attachment ) const noexcept {
		switch ( attachment ) {
			case DeferredAttachment::Albedo:
				return m_AlbedoAttachment;
			case DeferredAttachment::Depth:
				return m_DepthAttachment;
			default:
			break;
		}
		return m_AlbedoAttachment;
	}

	void DeferredBuffer::create_assets( SafePtr<rhi::IRenderDevice> renderDevice, rhi::Extent2D extent ) {

		renderDevice( ).DestroyImage( m_AlbedoAttachment );

		rhi::ImageCreateInfo info{};
		info.m_ArrayLayers = 1;
		info.m_MipmapLevels = 1;
		info.m_Extent = { extent.m_Width, extent.m_Height, 1 };
		info.m_Dimension = rhi::ImageDimension::Image2D;
		info.m_Format = rhi::ImageFormat::RGBA16_FLOAT;
		info.m_SampleCount = rhi::SampleCount::Sample1;
		info.m_ViewType = rhi::ImageViewType::Default;
		info.m_UsageFlags = rhi::ImageUsageFlag::ColorAttachment | rhi::ImageUsageFlag::ShaderSampled;

		m_AlbedoAttachment = renderDevice( ).CreateImage( info );
		LUM_ASSERT( m_AlbedoAttachment, "Invalid albedo attachment in DeferredBuffer" );

	}

} // namespace lum
