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

#include "Core/CoreTypes.hpp"
#include "Core/Utils/Flags.hpp"
#include "Core/Utils/HandlePool.hpp"
#include "Core/Utils/Optional.hpp"
#include "Core/Utils/ResourceLoader.hpp"

namespace lum::rhi {

	using ImageID = uint32;
	struct ImageHandle : BaseHandle<ImageID> {};

	enum class ImageUsageFlag : bitfield {

		None = 0,

		TransferSource = 1 << 0,
		TransferDestination = 1 << 1,

		ShaderReadable = 1 << 2, // Sampled
		ShaderReadWrite = 1 << 3, // Storage

		ColorAttachment = 1 << 4,
		DepthStencilAttachment = 1 << 5,

		InputAttachment = 1 << 6,

		TransientAttachment = 1 << 7

	};
	using ImageUsageFlags = Flags<ImageUsageFlag>;

	enum class ImageDimension : uint8 {
		Image1D = 0U,
		Image2D,
		Image3D,
	};

	enum class ImageViewType : uint8 {
		Default = 0U,
		ImageCube,
		ImageArray
	};

	enum class ImageFormat : uint8 {

		R8_UNORM = 0U,
		R8_SNORM,
		R8_UINT,
		R8_SINT,
		R8_SRGB,

		RG8_UNORM,
		RG8_SNORM,
		RG8_UINT,
		RG8_SINT,
		RG8_SRGB,

		RGB8_UNORM,
		RGB8_SNORM,
		RGB8_UINT,
		RGB8_SINT,
		RGB8_SRGB,

		RGBA8_UNORM,
		RGBA8_SNORM,
		RGBA8_UINT,
		RGBA8_SINT,
		RGBA8_SRGB,


		R16_UNORM,
		R16_SNORM,
		R16_UINT,
		R16_SINT,
		R16_FLOAT,

		RG16_UNORM,
		RG16_SNORM,
		RG16_UINT,
		RG16_SINT,
		RG16_FLOAT,

		RGB16_UNORM,
		RGB16_SNORM,
		RGB16_UINT,
		RGB16_SINT,
		RGB16_FLOAT,

		RGBA16_UNORM,
		RGBA16_SNORM,
		RGBA16_UINT,
		RGBA16_SINT,
		RGBA16_FLOAT,


		R32_UINT,
		R32_SINT,
		R32_FLOAT,

		RG32_UINT,
		RG32_SINT,
		RG32_FLOAT,

		RGB32_UINT,
		RGB32_SINT,
		RGB32_FLOAT,

		RGBA32_UINT,
		RGBA32_SINT,
		RGBA32_FLOAT,

		D16_UNORM,
		D32_FLOAT,

		D24_UNORM_S8_UINT,
		D32_FLOAT_S8_UINT,

	};
	
	enum class ImageLayout : uint8 {
		Undefined = 0U,             // Layout is not defined
		General,                    // General read and write access
		ColorAttachment,            // Used as a color attachment
		DepthStencilAttachment,     // Used as a depth/stencil attachment
		DepthStencilReadOnly,       // Used for depth/stencil read access
		ShaderReadOnly,             // Used for shader read access
		TransferSource,             // Used as a copy source
		TransferDestination,        // Used as a copy destination
		Present                     // Used for presenting to the screen
	};

	struct Extent3D {

		uint32 m_Width = 0;
		uint32 m_Height = 0;
		uint32 m_Depth = 0;

	};

	enum class SampleCount : uint8 {
		Sample1 = 0U,
		Sample2,
		Sample4,
		Sample8,
		Sample16,
		Sample32,
		Sample64
	};

	struct ImageCreateInfo {

		ImageUsageFlags m_UsageFlags = ImageUsageFlag::None;
		ImageDimension m_Dimension = ImageDimension::Image2D;
		ImageFormat m_Format = ImageFormat::RGBA32_FLOAT;
		Extent3D m_Extent{};
		ImageViewType m_ViewType = ImageViewType::Default;
		SampleCount m_SampleCount = SampleCount::Sample1;

		uint32 m_MipmapLevels = 1;
		uint32 m_ArrayLayers = 1;

	};

	struct ImageInfo {

		ImageUsageFlags m_UsageFlags = ImageUsageFlag::None;
		ImageLayout m_CurrentLayout = ImageLayout::Undefined;
		ImageFormat m_Format = ImageFormat::RGBA32_FLOAT;
		Extent3D m_Extent{};

		ImageViewType m_ViewType = ImageViewType::Default;
		SampleCount m_SampleCount = SampleCount::Sample1;

		uint32 m_MipmapLevels = 1;
		uint32 m_ArrayLayers = 1;

	};

} // namespace lum::rhi