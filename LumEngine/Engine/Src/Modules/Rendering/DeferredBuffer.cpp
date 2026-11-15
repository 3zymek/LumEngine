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

	void DeferredBuffer::Initialize( RendererContext& ctx, uint32 width, uint32 height ) {

		ctx.Assert( );

		m_Ctx = ctx;

		create_assets( width, height );

	}

	rhi::ImageHandle DeferredBuffer::GetAttachment( DeferredAttachment attachment ) const noexcept {
		switch ( attachment ) {
			case DeferredAttachment::Albedo:
			{
				return m_AlbedoAttachment;
			}

			default:
			break;
		}
		return m_AlbedoAttachment;
	}

	void DeferredBuffer::create_assets( uint32 width, uint32 height ) {

		m_Ctx( ).m_RenderDevice( ).DestroyImage( m_AlbedoAttachment );

		rhi::ImageCreateInfo info{};
		info.m_ArrayLayers = 1;
		info.m_MipmapLevels = 1;
		info.m_Extent = { width, height, 1 };
		info.m_Dimension = rhi::ImageDimension::Image2D;
		info.m_Format = rhi::ImageFormat::RGBA16_FLOAT;
		info.m_SampleCount = rhi::SampleCount::Sample1;
		info.m_ViewType = rhi::ImageViewType::Default;
		info.m_UsageFlags = rhi::ImageUsageFlag::ColorAttachment | rhi::ImageUsageFlag::ShaderSampled;

		m_AlbedoAttachment = m_Ctx( ).m_RenderDevice( ).CreateImage( info );
		LUM_ASSERT( m_AlbedoAttachment, "Invalid albedo attachment in DeferredBuffer" );

	}

} // namespace lum